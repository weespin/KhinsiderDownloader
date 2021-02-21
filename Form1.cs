using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Threading.Tasks;
using System.Windows.Forms;
using HtmlAgilityPack;

namespace KhinsiderDownloader
{
	public partial class Form1 : Form
	{
		
		public Form1()
		{
			InitializeComponent();
			Program.MainForm = this;
			lbl_path.Text = Downloader.sDownloadPath;
			LoadConfig();
		}

		public void Log(string textIn)
		{
			if (txt_log.InvokeRequired)
			{
				txt_log.Invoke(new Action(() =>
				{
					txt_log.Text += $"{textIn}\r\n";
					txt_log.SelectionStart = txt_log.Text.Length;
					txt_log.ScrollToCaret();
				}));
			}
		}

		public void LoadConfig()
		{
			try
			{
				var configLines = File.ReadAllLines("khinsiderdl.config");
				lbl_path.Text = Downloader.sDownloadPath = configLines[0];

				Downloader.eQuality = bool.Parse(configLines[1])
					? Downloader.EDownloadQuality.QUALITY_MP3_ONLY
					: Downloader.EDownloadQuality.QUALITY_BEST_ONLY;
				if (Downloader.eQuality == Downloader.EDownloadQuality.QUALITY_MP3_ONLY)
				{
					radio_mp3only.Checked = true;
				}
				else
				{
					radio_betterquality.Checked = true;
				}
			}
			catch (Exception)
			{
				Program.MainForm.Log("Failed to load a config!");
			}
		}

		void SaveConfig()
		{
			string[] configLines = new string[2];
			configLines[0] = Downloader.sDownloadPath;
			configLines[1] = (Downloader.eQuality == Downloader.EDownloadQuality.QUALITY_MP3_ONLY).ToString();
			File.WriteAllLines("khinsiderdl.config", configLines);
		}

		private void button1_Click(object sender, EventArgs e)
		{
			btn_download.Enabled = false;
			List<string> urls = txt_urllist.Text.Split('\n').ToList();
			Task.Run(() =>
			{
				foreach (var url in urls)
				{
					Downloader.DownloadAlbum(url);
				}
			}).ContinueWith((task =>
			{
				if (btn_download.InvokeRequired)
				{
					btn_download.Invoke(new Action(() =>
					{
						btn_download.Enabled = true;
					}));
				}
			}));
		}

		private void radioButton1_CheckedChanged(object sender, EventArgs e)
		{
			Downloader.eQuality = radio_mp3only.Checked
				? Downloader.EDownloadQuality.QUALITY_MP3_ONLY
				: Downloader.EDownloadQuality.QUALITY_BEST_ONLY;
			SaveConfig();
		}

		private void btn_DownloadPath_Click(object sender, EventArgs e)
		{
			using (var browserDialog = new FolderBrowserDialog())
			{
				DialogResult result = browserDialog.ShowDialog();

				if (result == DialogResult.OK && !string.IsNullOrWhiteSpace(browserDialog.SelectedPath))
				{
					lbl_path.Text = Downloader.sDownloadPath = browserDialog.SelectedPath;
					SaveConfig();
				}
			}
		}

		private void label1_DoubleClick(object sender, EventArgs e)
		{
			System.Diagnostics.Process.Start("https://github.com/weespin");
		}


		private void btn_opensearch_Click(object sender, EventArgs e)
		{
			SearchForm searchForm = new SearchForm();
			searchForm.linkbox = txt_urllist ;
			searchForm.Show();
		}
	}


	static class Downloader
	{
		public enum EDownloadQuality : byte
		{
			QUALITY_MP3_ONLY,
			QUALITY_BEST_ONLY
		}

		public static EDownloadQuality eQuality = EDownloadQuality.QUALITY_MP3_ONLY;
		public static string sDownloadPath = Directory.GetCurrentDirectory() + "\\Downloads\\";

		public static void DownloadAlbum(string sUrl)
		{
			if (!Directory.Exists(Downloader.sDownloadPath))
			{
				Directory.CreateDirectory(Downloader.sDownloadPath);
			}
			HtmlWeb webContext = new HtmlWeb();
			var htmlDocument = webContext.Load(sUrl);

			if (htmlDocument == null)
			{
				Program.MainForm.Log($"Failed to load {sUrl}");
				return;
			}

			Program.MainForm.Log($"Parsing {sUrl}");
			var songNodes = htmlDocument.DocumentNode.SelectNodes("//td[contains(@class, 'playlistDownloadSong')]");
			var qualityNodes = htmlDocument.DocumentNode.SelectNodes("//tr[contains(@id, 'songlist_header')]/th");
			var albumNameNode = htmlDocument.DocumentNode.SelectSingleNode("//*[@id=\"EchoTopic\"]/h2[1]");
			string albumName = "error";
			if (albumNameNode != null)
			{
				albumName = albumNameNode.InnerText;
			}
			else
			{
				Program.MainForm.Log($"Failed to parse {sUrl}");
				return;
			}
			Directory.CreateDirectory(Downloader.sDownloadPath + "\\" + albumName);
			var selectedFormat = ".mp3";
			if (eQuality != EDownloadQuality.QUALITY_MP3_ONLY)
			{
				List<HtmlNode> qualitySubNodes = qualityNodes.Skip(3).ToList();
				//find non mp3 file
				foreach (var cell in qualitySubNodes)
				{
					if (cell.InnerText == "FLAC")
					{
						selectedFormat = ".flac";
						break;
					}

					if (cell.InnerText == "OGG")
					{
						selectedFormat = ".ogg";
						break;
					}
					if (cell.InnerText == "M4A")
					{
						selectedFormat = ".m4a";
						break;
					}
				}
			}

			//Thread.Sleep(1);

			List<Task> currentTasks = new List<Task>();

			songNodes.AsParallel().ForAll(song =>
			{
				var songPageURL = "https://downloads.khinsider.com" + song.ChildNodes[0].Attributes["href"].Value;
				
				var songPageDocument = webContext.Load(songPageURL);
				
				var downloadLinkNodes = songPageDocument.DocumentNode.SelectNodes("//span[@class='songDownloadLink']"); //[1].ParentElement.GetAttribute("href");
				foreach (var dlsongentry in downloadLinkNodes)
				{
					var songFileURL = dlsongentry.ParentNode.Attributes["href"].Value;
					if (songFileURL.EndsWith(selectedFormat))
					{
						WebClient downloadClient = new WebClient();
						downloadClient.Proxy = null;
						var name = WebUtility.UrlDecode(songFileURL.Substring(songFileURL.LastIndexOf("/") + 1));
						Program.MainForm.Log($"Downloading {name}...");
						Task currentTask = downloadClient.DownloadFileTaskAsync(new Uri(songFileURL),
							Downloader.sDownloadPath + "\\" +
							string.Join("_", albumName.Split(Path.GetInvalidFileNameChars())) + "\\" +
							string.Join("_", name.Split(Path.GetInvalidFileNameChars())));
						currentTasks.Add(currentTask);
						currentTask.ContinueWith((
							task => { Program.MainForm.Log($"{name} has been downloaded!"); }));
					}
				}
			});
			Task.WaitAll(currentTasks.ToArray());
			Program.MainForm.Log($"Finished downloading {albumName}!");
		}
	}
}