using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Resources;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using AngleSharp;
using AngleSharp.Dom;
using AngleSharp.Html.Dom;
using AngleSharp.Html.Parser;
using Newtonsoft.Json;

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
			LoadConfig();
			lbl_path.Text = Downloader.m_szDownloadPath;
			Downloader.g_songsParralelOptions.MaxDegreeOfParallelism = 2;
			Downloader.g_albumsParralelOptions.MaxDegreeOfParallelism = 1;
			num_threads.Value = 2;
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
				lbl_path.Text = Downloader.m_szDownloadPath = configLines[0];
				
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

				Downloader.g_songsParralelOptions.MaxDegreeOfParallelism = Int32.Parse(configLines[2]);
				Downloader.g_albumsParralelOptions.MaxDegreeOfParallelism = Int32.Parse(configLines[3]);
				Downloader.m_bSuppessLogs = bool.Parse(configLines[4]);


			}
			catch (Exception)
			{
				Program.MainForm.Log("Failed to load a config!");
			}
		}

		void SaveConfig()
		{
			string[] configLines = new string[5];
			configLines[0] = Downloader.m_szDownloadPath;
			configLines[1] = (Downloader.eQuality == Downloader.EDownloadQuality.QUALITY_MP3_ONLY).ToString();
			configLines[2] = Downloader.g_songsParralelOptions.MaxDegreeOfParallelism.ToString();
			configLines[3] = Downloader.g_albumsParralelOptions.MaxDegreeOfParallelism.ToString();
			configLines[4] = Downloader.m_bSuppessLogs.ToString();


			File.WriteAllLines("khinsiderdl.config", configLines);
		}

		void ToggleControls(bool value)
		{
			btn_download.Enabled = value;
			radio_betterquality.Enabled = value;
			radio_mp3only.Enabled = value;
			btn_selectpath.Enabled = value;
			num_album_threads.Enabled = value;
			num_threads.Enabled = value;
		}
		private void button1_Click(object sender, EventArgs e)
		{
			ToggleControls(false);
			List<string> urls = txt_urllist.Text.Split(new string[] {Environment.NewLine }, StringSplitOptions.RemoveEmptyEntries).ToList();
			Task.Run(() =>
			{
				Downloader.DownloadAlbums(urls);
				
			}).ContinueWith((task =>
			{
				if (btn_download.InvokeRequired)
				{
					btn_download.Invoke(new Action(() =>
					{
						ToggleControls(true);
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
					lbl_path.Text = Downloader.m_szDownloadPath = browserDialog.SelectedPath;
					SaveConfig();
				}
			}
		}

		private void label1_DoubleClick(object sender, EventArgs e)
		{
			Process.Start("https://github.com/weespin");
		}


		private void btn_opensearch_Click(object sender, EventArgs e)
		{
			SearchForm searchForm = new SearchForm();
			searchForm.linkbox = txt_urllist;
			searchForm.Show();
		}

		private void num_threads_ValueChanged(object sender, EventArgs e)
		{
			Downloader.g_songsParralelOptions.MaxDegreeOfParallelism = (int)num_threads.Value;
			SaveConfig();
		}

		private void num_album_threads_ValueChanged(object sender, EventArgs e)
		{
			Downloader.g_albumsParralelOptions.MaxDegreeOfParallelism = (int)num_album_threads.Value;
			SaveConfig();
		}

		private void chk_suppress_downloading_logs_CheckedChanged(object sender, EventArgs e)
		{
			Downloader.m_bSuppessLogs = chk_suppress_downloading_logs.Checked;
		}
	}


	static class Downloader
	{
		//ParralelOptions for songs in album
		public static ParallelOptions g_songsParralelOptions = new ParallelOptions()
			{MaxDegreeOfParallelism = Environment.ProcessorCount};
		//ParralelOptions for albums
		public static ParallelOptions g_albumsParralelOptions = new ParallelOptions()
			{ MaxDegreeOfParallelism = Environment.ProcessorCount };
		public static int nTotalAlbums = 0;
		public static int nAlbumsDownloaded = 0;

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
		public static string m_szDownloadPath = Directory.GetCurrentDirectory() + "\\Downloads\\";
		public static bool m_bSuppessLogs;

		public static void DownloadAlbums(List<string> url)
		{

			if (!Directory.Exists(Downloader.m_szDownloadPath))
			{
				Directory.CreateDirectory(Downloader.m_szDownloadPath);
			}

			var maxthreads = g_songsParralelOptions.MaxDegreeOfParallelism * g_albumsParralelOptions.MaxDegreeOfParallelism;
			ThreadPool.SetMinThreads(maxthreads, maxthreads);
			System.Net.ServicePointManager.DefaultConnectionLimit = Int32.MaxValue;
			nAlbumsDownloaded = 0;
			nTotalAlbums = url.Count;
			if (g_albumsParralelOptions.MaxDegreeOfParallelism != 1)
			{
				UpdateTitle(nAlbumsDownloaded, nTotalAlbums);
			}
			Parallel.ForEach(url,g_albumsParralelOptions, DownloadAlbum);
			ResetTitle();
		}

		public static string GetHTMLFromURL(string sUrl)
		{
			HttpWebRequest httpWebRequest = (HttpWebRequest)WebRequest.Create(sUrl);
			httpWebRequest.Proxy = null;
			httpWebRequest.KeepAlive = false;
			httpWebRequest.Timeout = 30 * 1000; //TCP timeout
			using (HttpWebResponse httpWebResponse = (HttpWebResponse)httpWebRequest.GetResponse())
			{
				if (httpWebResponse.StatusCode == HttpStatusCode.OK)
				{
					using (Stream responseStream = httpWebResponse.GetResponseStream())
					{
						if (responseStream != null)
						{
							using (StreamReader reader = new StreamReader(responseStream))
							{
								return reader.ReadToEnd();
							}
						}
					}
				}
			}
			return string.Empty;
		}
		public static void DownloadAlbum(string sUrl)
		{
			string albumHTML = String.Empty;
			try
			{
				albumHTML = GetHTMLFromURL(sUrl);
			}
			catch (Exception e)
			{
				string errorMessage = $"Failed to open {sUrl} ({e.Message})";
				Program.MainForm.Log(errorMessage);
#if DEBUG
				Debug.WriteLine(errorMessage);
#endif
			}


			var parser = new HtmlParser();
			var albumHtmlDocument = parser.ParseDocument(albumHTML);
			Program.MainForm.Log($"Parsing {sUrl}");
			var songNodes = albumHtmlDocument.All.Where(element =>element.LocalName == "td" && element.ClassName == "playlistDownloadSong");//.SelectNodes("//td[contains(@class, 'playlistDownloadSong')]");
			var qualityNodes = albumHtmlDocument.All.Where(element =>element.LocalName == "tr" && element.Id == "songlist_header"); //DocumentNode.SelectNodes("//tr[contains(@id, 'songlist_header')]/th");
			var albumNameNode = albumHtmlDocument.All.FirstOrDefault(element => element.LocalName == "div" && element.Id == "EchoTopic")?.Children[1];  //DocumentNode.SelectSingleNode("//*[@id=\"EchoTopic\"]/h2[1]");
			string szAlbumName;
			if (albumNameNode != null && songNodes.Any() && qualityNodes.Any())
			{
				//Trim spaces and dots!
				szAlbumName = string.Join("_", WebUtility.HtmlDecode(albumNameNode.InnerHtml).Split(Path.GetInvalidFileNameChars())).Trim(new char[]{' ','.'});
			}
			else
			{
				Program.MainForm.Log($"Failed to parse {sUrl}");
				return;
			}
			
			Directory.CreateDirectory(Downloader.m_szDownloadPath + "\\" + szAlbumName);
			var selectedFormat = ".mp3";
			if (eQuality != EDownloadQuality.QUALITY_MP3_ONLY)
			{
				List<IElement> qualitySubNodes = qualityNodes.Skip(3).ToList();
				//find non mp3 file
				foreach (var cell in qualitySubNodes)
				{
					if (cell.InnerHtml == "FLAC")
					{
						selectedFormat = ".flac";
						break;
					}
			
					if (cell.InnerHtml == "OGG")
					{
						selectedFormat = ".ogg";
						break;
					}
					if (cell.InnerHtml == "M4A")
					{
						selectedFormat = ".m4a";
						break;
					}
				}
			}
			List<Task> currentTasks = new List<Task>();
			int nTotalSongs = songNodes.Count();
			int nCurrentSong = 0;

			Parallel.ForEach(songNodes, g_songsParralelOptions,song=>
			{
				var songPageURL =
					"https://downloads.khinsider.com" + song.Children[0].GetAttribute("href"); //["href"].Value;

				IHtmlDocument songPageDocument;

				try
				{
					if (g_songsParralelOptions.MaxDegreeOfParallelism == 1)
					{
						songPageDocument = new HtmlParser().ParseDocument(GetHTMLFromURL(songPageURL));
					}
					else
					{
						songPageDocument = parser.ParseDocument(GetHTMLFromURL(songPageURL)); 
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

				var downloadLinkNodes =
					songPageDocument.All.Where(element=>element.ClassName == "songDownloadLink").ToList(); //"//span[@class='songDownloadLink']"); //[1].ParentElement.GetAttribute("href");
				//Do not use Parallel.ForEach as we usually have ~2 nodes
				int nDownloadNodes = downloadLinkNodes.Count();
				for (var index = 0; index < nDownloadNodes; index++)
				{
					var dlsongentry = downloadLinkNodes[index];
					var songFileURL = dlsongentry.ParentElement.GetAttribute("href"); //.Value;
					if (songFileURL.EndsWith(selectedFormat))
					{
						var name = WebUtility.HtmlDecode(
							songFileURL.Substring(songFileURL.LastIndexOf("/", StringComparison.Ordinal) + 1));
						if (!m_bSuppessLogs)
						{
							Program.MainForm.Log($"Downloading {name}...");
						}
					
						string filename = m_szDownloadPath + "\\" + szAlbumName + "\\" + string.Join("_", name.Split(Path.GetInvalidFileNameChars()));
						try
						{
					
							WebClient downloadClient = new WebClient();
							downloadClient.Proxy = null;
							Task currentTask = downloadClient.DownloadFileTaskAsync(new Uri(songFileURL), filename);
							currentTasks.Add(currentTask);
							currentTask.ContinueWith(
								task =>
								{
									downloadClient.Dispose();
									++nCurrentSong;
									if (g_albumsParralelOptions.MaxDegreeOfParallelism == 1)
									{
										UpdateTitle(nCurrentSong, nTotalSongs);
									}
					
									if (!m_bSuppessLogs)
									{
										Program.MainForm.Log($"{name} has been downloaded!");
									}
								});
					
						}
						catch (Exception e)
						{
							string errorMessage = $"Failed to download {songFileURL} to {filename} ({e.Message})";
							Program.MainForm.Log(errorMessage);
#if DEBUG 
							Debug.WriteLine(errorMessage);
#endif 
						}
					}
				}
			});
			Task.WaitAll(currentTasks.ToArray());
			Program.MainForm.Log($"Finished downloading {szAlbumName}!");
			if (g_albumsParralelOptions.MaxDegreeOfParallelism != 1)
			{
				UpdateTitle(++nAlbumsDownloaded, nTotalAlbums);
			}
		}
	}
}