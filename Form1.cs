using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using AngleSharp;
using AngleSharp.Dom;

//./Downloads/...
namespace KhinsiderDownloader
{
	public partial class Form1 : Form
	{
		
		public Form1()
		{
			InitializeComponent();
			Program.MainForm = this;
			downloadPath_label.Text = Downloader.sDownloadPath;
			LoadConfig();
		}

		public void Log(string sText)
		{
			if (textBox2.InvokeRequired)
			{
				textBox2.Invoke(new Action(() =>
				{
					textBox2.Text += $"{sText}\r\n";
					textBox2.SelectionStart = textBox2.Text.Length;
					textBox2.ScrollToCaret();
				}));
			}
		}

		public void LoadConfig()
		{
			try
			{
				var lines = File.ReadAllLines("khinsiderdl.config");
				downloadPath_label.Text = Downloader.sDownloadPath = lines[0];

				Downloader.eQuality = bool.Parse(lines[1])
					? Downloader.EDownloadQuality.QUALITY_MP3_ONLY
					: Downloader.EDownloadQuality.QUALITY_BEST_ONLY;
				if (Downloader.eQuality == Downloader.EDownloadQuality.QUALITY_MP3_ONLY)
				{
					radioButton1.Checked = true;
				}
				else
				{
					radioButton2.Checked = true;
				}
			}
			catch (Exception)
			{
				Program.MainForm.Log("Failed to load a config!");
			}
		}

		void SaveConfig()
		{
			string[] cfglines = new string[2];
			cfglines[0] = Downloader.sDownloadPath;
			cfglines[1] = (Downloader.eQuality == Downloader.EDownloadQuality.QUALITY_MP3_ONLY).ToString();
			File.WriteAllLines("khinsiderdl.config", cfglines);
		}

		private void button1_Click(object sender, EventArgs e)
		{
			button1.Enabled = false;
			List<string> urls = urllist_box.Text.Split('\n').ToList();
			Task.Run(() =>
			{
				foreach (var url in urls)
				{
					Downloader.DownloadAlbum(url);
				}
			}).ContinueWith((task =>
			{
				if (button1.InvokeRequired)
				{
					button1.Invoke(new Action(() =>
					{
						button1.Enabled = true;
					}));
				}
			}));
		}

		private void radioButton1_CheckedChanged(object sender, EventArgs e)
		{
			Downloader.eQuality = radioButton1.Checked
				? Downloader.EDownloadQuality.QUALITY_MP3_ONLY
				: Downloader.EDownloadQuality.QUALITY_BEST_ONLY;
			SaveConfig();
		}

		private void btn_DownloadPath_Click(object sender, EventArgs e)
		{
			using (var fbd = new FolderBrowserDialog())
			{
				DialogResult result = fbd.ShowDialog();

				if (result == DialogResult.OK && !string.IsNullOrWhiteSpace(fbd.SelectedPath))
				{
					downloadPath_label.Text = Downloader.sDownloadPath = fbd.SelectedPath;
					SaveConfig();
				}
			}
		}

		private void label1_DoubleClick(object sender, EventArgs e)
		{
			System.Diagnostics.Process.Start("https://github.com/weespin");
		}

		private void Form1_Load(object sender, EventArgs e)
		{
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

			var context = BrowsingContext.New(Configuration.Default.WithDefaultLoader());
			var document = context.OpenAsync(sUrl).Result;
			if (document == null)
			{
				Program.MainForm.Log($"Failed to load {sUrl}");
				return;
			}

			Program.MainForm.Log($"Parsing {sUrl}");
			var songs = document.QuerySelectorAll(".playlistDownloadSong");
			var quality = document.QuerySelectorAll("#songlist_header");
			string albumname = "";
			try
			{
				albumname = document.QuerySelectorAll("#EchoTopic")[0].Children[1].Text();
			}
			catch (Exception exception)
			{
				Program.MainForm.Log($"Failed to parse {sUrl}");
				return;
			}

			Directory.CreateDirectory(Downloader.sDownloadPath + "\\" + albumname);
			var format = ".mp3";
			if (eQuality != EDownloadQuality.QUALITY_MP3_ONLY)
			{
				//find non mp3 file
				foreach (var cell in quality[0].Children.Skip(2).ToList())
				{
					if (cell.TextContent == "FLAC")
					{
						format = ".flac";
						break;
					}

					if (cell.TextContent == "OGG")
					{
						format = ".ogg";
						break;
					}
					if (cell.TextContent == "M4A")
					{
						format = ".m4a";
						break;
					}
				}
			}

			List<Task> currentTasks = new List<Task>();
			songs.AsParallel().ForAll(song =>
			{
				var child = "https://downloads.khinsider.com" + song.Children[0].GetAttribute("href");
				var document2 = context.OpenAsync(child).Result;
				var dlsongs = document2.QuerySelectorAll(".songDownloadLink"); //[1].ParentElement.GetAttribute("href");
				foreach (var dlsongentry in dlsongs)
				{
					var dlsong = dlsongentry.ParentElement.GetAttribute("href");
					if (dlsong.EndsWith(format))
					{
						var name = WebUtility.UrlDecode(dlsong.Substring(dlsong.LastIndexOf("/") + 1));
						WebClient cl = new WebClient();
						cl.Proxy = null;
						Program.MainForm.Log($"Downloading {name}...");
						Task currentTask = cl.DownloadFileTaskAsync(new Uri(dlsong),
							Downloader.sDownloadPath + "\\" +
							string.Join("_", albumname.Split(Path.GetInvalidFileNameChars())) + "\\" +
							string.Join("_", name.Split(Path.GetInvalidFileNameChars())));
						currentTasks.Add(currentTask);
						currentTask.ContinueWith((
							task => { Program.MainForm.Log($"{name} has been downloaded!"); }));
					}
				}
			});
			Task.WaitAll(currentTasks.ToArray());
			Program.MainForm.Log($"Finished downloading {albumname}!");
			//}
		}
	}
}