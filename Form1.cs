using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using HtmlAgilityPack;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using HtmlDocument = HtmlAgilityPack.HtmlDocument;

namespace KhinsiderDownloader
{
	public partial class Form1 : Form
	{
		
		public static string programName = "KhinsiderDownloader";
		class UpdateTagResult
		{
			public string tag_name { get; set; }
		}
		private void checkUpdates()
		{
			WebClient cl = new WebClient();
			cl.Headers.Add("User-Agent", "KhinsiderDownloader");
			cl.Headers.Add("Content-Type", "application/json");
			string lastVersion = cl.DownloadString(new Uri("https://api.github.com/repos/weespin/KhinsiderDownloader/releases/latest"));
			var result = JsonConvert.DeserializeObject<UpdateTagResult>(lastVersion);
			Version newVersion = new Version(result.tag_name);
			Version currentVersion = new Version(Application.ProductVersion);

			if (newVersion > currentVersion)
			{
				var promptResult = MessageBox.Show(
					"Download the latest version of KhinsiderDownloader!\nClick OK to open the download page",
					"A new update has been released!", MessageBoxButtons.OKCancel);
				if (promptResult == DialogResult.OK)
				{
					Process.Start("https://github.com/weespin/KhinsiderDownloader/releases");
				}
			}
			else if (newVersion < currentVersion)
			{
				if (label1.InvokeRequired)
				{
					label1.Invoke(new Action(() =>
					{
						label1.Text = "DEV " + label1.Text;
					}));

				}
			}

		}

		public Form1()
		{
			InitializeComponent();
			Program.MainForm = this;
			lbl_path.Text = Downloader.sDownloadPath;
			Downloader.g_parralelopt.MaxDegreeOfParallelism = 2;
			num_threads.Value = 2;
			LoadConfig();
			Task.Run(() => { checkUpdates(); });
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

				Downloader.g_parralelopt.MaxDegreeOfParallelism = Int32.Parse(configLines[2]);
			}
			catch (Exception)
			{
				Program.MainForm.Log("Failed to load a config!");
			}
		}

		void SaveConfig()
		{
			string[] configLines = new string[3];
			configLines[0] = Downloader.sDownloadPath;
			configLines[1] = (Downloader.eQuality == Downloader.EDownloadQuality.QUALITY_MP3_ONLY).ToString();
			configLines[2] = Downloader.g_parralelopt.MaxDegreeOfParallelism.ToString();
			File.WriteAllLines("khinsiderdl.config", configLines);
		}

		private void button1_Click(object sender, EventArgs e)
		{
			btn_download.Enabled = false;
			radio_betterquality.Enabled = false;
			radio_mp3only.Enabled = false;
			btn_selectpath.Enabled = false;
			List<string> urls = txt_urllist.Text.Split(new string[] {Environment.NewLine }, StringSplitOptions.RemoveEmptyEntries).ToList();
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
						radio_betterquality.Enabled = true;
						radio_mp3only.Enabled = true;
						btn_selectpath.Enabled = true;

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

		private void num_threads_ValueChanged(object sender, EventArgs e)
		{
			Downloader.g_parralelopt.MaxDegreeOfParallelism = (int)num_threads.Value;
			SaveConfig();
		}
	}


	static class Downloader
	{
		public static ParallelOptions g_parralelopt = new ParallelOptions()
			{MaxDegreeOfParallelism = Environment.ProcessorCount};
		static void UpdateTitle(int value,int max)
		{
			Program.MainForm.Invoke(new Action(() =>
			{
				Program.MainForm.Text = $"{Form1.programName} ({value}/{max})";
			}));
		
		}

		static void ResetTitle()
		{
			Program.MainForm.Invoke(new Action(() =>
			{
				Program.MainForm.Text = $"{Form1.programName}";
			}));
		}
		public enum EDownloadQuality : byte
		{
			QUALITY_MP3_ONLY,
			QUALITY_BEST_ONLY
		}

		public static EDownloadQuality eQuality = EDownloadQuality.QUALITY_MP3_ONLY;
		public static string sDownloadPath = Directory.GetCurrentDirectory() + "\\Downloads\\";
		public static CancellationTokenSource cancelTokenSource;

		public static void DownloadAlbum(string sUrl)
		{
			cancelTokenSource = new CancellationTokenSource();
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
				albumName = string.Join("_", albumNameNode.InnerText.Split(Path.GetInvalidFileNameChars())).Trim();
				//albumName = albumNameNode.InnerText.Trim();
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
			ThreadPool.SetMinThreads(g_parralelopt.MaxDegreeOfParallelism, g_parralelopt.MaxDegreeOfParallelism);
			System.Net.ServicePointManager.DefaultConnectionLimit = Int32.MaxValue;
			//foreach (var song in songNodes)
			//{
			int tasknum = songNodes.Count;
			int currentnum = 0;
			Parallel.ForEach(songNodes, g_parralelopt,song=>
			{
				var songPageURL = "https://downloads.khinsider.com" + song.ChildNodes[0].Attributes["href"].Value;
				HtmlDocument songPageDocument = new HtmlDocument();
				try
				{
					HttpWebRequest httpWebRequest = (HttpWebRequest)WebRequest.Create(songPageURL);
					httpWebRequest.KeepAlive = false;
					httpWebRequest.Timeout = 30 * 1000; //TCP timeout
					using (HttpWebResponse httpWebResponse = (HttpWebResponse)httpWebRequest.GetResponse())
					{
						if (httpWebResponse.StatusCode == HttpStatusCode.OK)
						{
							using (Stream responseStream = httpWebResponse.GetResponseStream())
							{
								using (StreamReader reader = new StreamReader(responseStream))
								{
									var htmlstring = reader.ReadToEnd();

									songPageDocument.LoadHtml(htmlstring);
								}
							}

						}
					}
				}
				catch (Exception e)
				{
					string message = $"Failed to parse {songPageURL} ({e.Message})";
					Program.MainForm.Log(message);
#if DEBUG
					Debug.WriteLine(message);
#endif
					return;
				}
				
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
					
						string filename = Downloader.sDownloadPath + "\\" +
						                  albumName + "\\" +
						                  string.Join("_", name.Split(Path.GetInvalidFileNameChars()));

						try
						{
						Task currentTask = downloadClient.DownloadFileTaskAsync(new Uri(songFileURL), filename);
						currentTasks.Add(currentTask);
						currentTask.ContinueWith((
							task =>
							{
								++currentnum;
								UpdateTitle(currentnum, tasknum);
								Program.MainForm.Log($"{name} has been downloaded!");
							}));
						}
						catch (Exception e)
						{
							string message = $"Failed to download {songFileURL} to {filename} ({e.Message})";
							Program.MainForm.Log(message);
#if DEBUG
							Debug.WriteLine(message);
#endif
						}

					}
				}
				//}// for foreach
			});
			Task.WaitAll(currentTasks.ToArray(), cancelTokenSource.Token);
			if (cancelTokenSource.IsCancellationRequested)
			{
				Program.MainForm.Log($"Download was cancelled!");
			
			}
			else
			{
				Program.MainForm.Log($"Finished downloading {albumName}!");

			}

			ResetTitle();
		}

		
	}
}