using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using AngleSharp.Dom;
using AngleSharp.Html.Dom;
using AngleSharp.Html.Parser;
using Newtonsoft.Json;
//using TagLib;
//using TagLib.Mpeg;
//using TagLib.Id3v2;
using File = System.IO.File;
using Version = System.Version;
using AngleSharp.Io;
using System.Web;
using System.Xml.Linq;
using static System.Windows.Forms.LinkLabel;
using System.Security.Policy;
using System.Text.RegularExpressions;

// ReSharper disable ConvertToUsingDeclaration

namespace KhinsiderDownloader
{

    public partial class Form1 : Form
    {

        public static string programName = "KhinsiderDownloader";

        class UpdateTagResult
        {
            // ReSharper disable once InconsistentNaming
            public string tag_name { get; set; }
        }

        private void checkUpdates()
        {
            try
            {
                WebClient cl = new WebClient();
                cl.Headers.Add("User-Agent", "KhinsiderDownloader");
                cl.Headers.Add("Content-Type", "application/json");
                string lastVersion =
                    cl.DownloadString(new Uri("https://api.github.com/repos/weespin/KhinsiderDownloader/releases/latest"));
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
                        label1.Invoke(new Action(() => { label1.Text = "DEV " + label1.Text; }));

                    }
                }
            }
            catch (Exception e)
            {
                Log($"Failed to check for updates!");
            }
        }

        public Form1()
        {
            InitializeComponent();
            Program.MainForm = this;
          
            if (!LoadConfig())
            {
                Downloader.g_songsParralelOptions.MaxDegreeOfParallelism = (int)num_threads.Value;
                Downloader.g_albumsParralelOptions.MaxDegreeOfParallelism = 1;
            }

            lbl_path.Text = Downloader.m_szDownloadPath;
            Task.Run(() => { checkUpdates(); });

            Log($"KhinsiderDownloader - {new Version(Application.ProductVersion)}\r\nIf you encounter any problems, crashes, or have suggestions, please share your feedback on GitHub: https://github.com/weespin/KhinsiderDownloader/issues");
        }
        public void Log(string textIn)
        {
            var LogAction = new Action(() =>
            {
                txt_log.Text += $"{textIn}\r\n";
                txt_log.SelectionStart = txt_log.Text.Length;
                txt_log.ScrollToCaret();
            });

            if (txt_log.InvokeRequired)
            {
                //Called from a different thread
                txt_log.Invoke(LogAction);
            }
            else
            {
                LogAction();
            }
        }

        public bool LoadConfig()
        {
            if (!File.Exists("khinsiderdl.config"))
            {
                //Probably a first launch
                return false;
            }
            try
            {
                var configLines = File.ReadAllLines("khinsiderdl.config");
                lbl_path.Text = Downloader.m_szDownloadPath = configLines[0];
                
                Downloader.eQuality = bool.Parse(configLines[1])
                    ? Downloader.EDownloadQuality.QUALITY_MP3_ONLY
                    : Downloader.EDownloadQuality.QUALITY_BEST_ONLY;
                num_threads.Value = Downloader.g_songsParralelOptions.MaxDegreeOfParallelism = Int32.Parse(configLines[2]);
                num_album_threads.Value = Downloader.g_albumsParralelOptions.MaxDegreeOfParallelism = Int32.Parse(configLines[3]);
                chk_suppress_downloading_logs.Checked = Downloader.m_bSuppessLogs = bool.Parse(configLines[4]);
                chk_download_art.Checked = Downloader.m_bDownloadArt = bool.Parse(configLines[5]);
                chk_skipdownloaded.Checked = Downloader.m_bSkipDownloaded = bool.Parse(configLines[6]);
                //Downloader.m_bArtFix = bool.Parse(configLines[7]);

                if (Downloader.eQuality == Downloader.EDownloadQuality.QUALITY_MP3_ONLY)
                {
                    radio_mp3only.Checked = true;
                }
                else
                {
                    radio_betterquality.Checked = true;
                }

                return true;
            }
            catch (Exception)
            {
                Program.MainForm.Log("Failed to load a config!");
            }
            return false;
        }

        void SaveConfig()
        {
            string[] configLines = new string[7];
            configLines[0] = Downloader.m_szDownloadPath;
            configLines[1] = (Downloader.eQuality == Downloader.EDownloadQuality.QUALITY_MP3_ONLY).ToString();
            configLines[2] = Downloader.g_songsParralelOptions.MaxDegreeOfParallelism.ToString();
            configLines[3] = Downloader.g_albumsParralelOptions.MaxDegreeOfParallelism.ToString();
            configLines[4] = Downloader.m_bSuppessLogs.ToString();
            configLines[5] = Downloader.m_bDownloadArt.ToString();
            configLines[6] = Downloader.m_bSkipDownloaded.ToString();
            //configLines[7] = Downloader.m_bArtFix.ToString();
            File.WriteAllLines("khinsiderdl.config", configLines);
        }

        void ToggleControls(bool value)
        {
            btn_download.Text = value ? "Download" : "Cancel";
            radio_betterquality.Enabled = value;
            radio_mp3only.Enabled = value;
            btn_selectpath.Enabled = value;
            num_album_threads.Enabled = value;
            num_threads.Enabled = value;
            Downloader.IsDownloading = !value;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (Downloader.IsDownloading)
            {

                Downloader.cancelTokenSource.Cancel();
                ToggleControls(true);
            }
            
            ToggleControls(false);
            List<string> urls = txt_urllist.Text
                .Split(new string[] { Environment.NewLine }, StringSplitOptions.RemoveEmptyEntries).Distinct().ToList();
            Task.Run(() => { Downloader.DownloadAlbums(urls); }).ContinueWith((task =>
            {
                if (btn_download.InvokeRequired)
                {
                    btn_download.Invoke(new Action(() =>
                    {
                        ToggleControls(true);
                        TaskbarProgress.SetState(this.Handle, TaskbarProgress.TaskbarStates.NoProgress);
                        Downloader.cancelTokenSource = new CancellationTokenSource();
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
            SearchForm searchForm = new SearchForm() {linkbox = txt_urllist};
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
            SaveConfig();
        }

        private void chk_set_art_as_icon_CheckedChanged(object sender, EventArgs e)
        {
            //Downloader.m_bArtFix = chk_fix_picture.Checked;
        }
        private void chk_download_art_CheckedChanged(object sender, EventArgs e)
        {
            Downloader.m_bDownloadArt = chk_download_art.Checked;
            SaveConfig();
        }
        private void chk_skipdownloaded_CheckedChanged(object sender, EventArgs e)
        {
            Downloader.m_bSkipDownloaded = chk_skipdownloaded.Checked;
            SaveConfig();
        }

        private void lbl_path_Click(object sender, EventArgs e)
        {
            Process.Start(Downloader.m_szDownloadPath);
        }
    }

    static class Downloader
    {
        public static CancellationTokenSource cancelTokenSource = new CancellationTokenSource();

        public static bool IsDownloading = false;

        //ParallelOptions for songs in album
        public static ParallelOptions g_songsParralelOptions = new ParallelOptions()
            { MaxDegreeOfParallelism = Environment.ProcessorCount };

        //ParallelOptions for albums
        public static ParallelOptions g_albumsParralelOptions = new ParallelOptions()
            { MaxDegreeOfParallelism = Environment.ProcessorCount };

        public static int nTotalAlbums = 0;
        public static int nAlbumsDownloaded = 0;

        static void UpdateTitle(int value, int max)
        {
            Program.MainForm.Invoke(new Action(() =>
            {
                Program.MainForm.Text = $"{Form1.programName} ({value}/{max})";
                TaskbarProgress.SetValue(Program.MainForm.Handle, value, max);
            }));
        }

        static void ResetTitle()
        {
            Program.MainForm.Invoke(new Action(() => { Program.MainForm.Text = $"{Form1.programName}"; }));
        }

        public enum EDownloadQuality : byte
        {
            QUALITY_MP3_ONLY,
            QUALITY_BEST_ONLY
        }

        public static EDownloadQuality eQuality = EDownloadQuality.QUALITY_MP3_ONLY;
        public static string m_szDownloadPath = Directory.GetCurrentDirectory() + "\\Downloads\\";
        public static bool m_bSuppessLogs;
        public static bool m_bDownloadArt;
        public static bool m_bSkipDownloaded;

        //public static bool m_bArtFix;
        public static string m_szHostName = "https://downloads.khinsider.com";

        public static void DownloadAlbums(List<string> url)
        {
            g_albumsParralelOptions.CancellationToken = Downloader.cancelTokenSource.Token;
            g_songsParralelOptions.CancellationToken = Downloader.cancelTokenSource.Token;

            if (!Directory.Exists(Downloader.m_szDownloadPath))
            {
                Directory.CreateDirectory(Downloader.m_szDownloadPath);
            }

            var maxthreads = g_songsParralelOptions.MaxDegreeOfParallelism *
                             g_albumsParralelOptions.MaxDegreeOfParallelism;
            ThreadPool.SetMinThreads(maxthreads, maxthreads);
            System.Net.ServicePointManager.DefaultConnectionLimit = Int32.MaxValue;
            nAlbumsDownloaded = 0;
            nTotalAlbums = url.Count;
            if (nTotalAlbums != 1)
            {
                UpdateTitle(nAlbumsDownloaded, nTotalAlbums);
            }

            try
            {
                Parallel.ForEach(url, g_albumsParralelOptions, DownloadAlbum);
            }
            catch (OperationCanceledException)
            {

            }

            if (cancelTokenSource.IsCancellationRequested)
            {
                Program.MainForm.Log("Download was cancelled");
            }

            ResetTitle();
        }

        public struct HTMLResult
        {
            public string ResponseURI;
            public string HTML;
        }
        public static HTMLResult GetHTMLFromURL(string sUrl)
        {
            HTMLResult result = new HTMLResult();
            HttpWebRequest httpWebRequest = (HttpWebRequest)WebRequest.Create(sUrl);
            httpWebRequest.Proxy = null;
            httpWebRequest.KeepAlive = false;
            httpWebRequest.Timeout = 30 * 1000; //TCP timeout
            using (HttpWebResponse httpWebResponse = (HttpWebResponse)httpWebRequest.GetResponse())
            {
                if (httpWebResponse.StatusCode == HttpStatusCode.OK)
                {
                    result.ResponseURI = httpWebResponse.ResponseUri.ToString();

                    using (Stream responseStream = httpWebResponse.GetResponseStream())
                    {
                        if (responseStream != null)
                        {
                            using (StreamReader reader = new StreamReader(responseStream))
                            {
                                result.HTML = reader.ReadToEnd();
                            }
                        }
                    }
                }
            }

            return result;
        }
        public static void DownloadAlbum(string sUrl)
        {
            bool bIsPlaylist = false;
            string pattern = @"\/playlist\/\w+";
            Regex regex = new Regex(pattern);
            bIsPlaylist = regex.Match(sUrl).Success;

            SynchronizedCollection<Task> currentTasks = new SynchronizedCollection<Task>();

            string albumHTML = String.Empty;
            try
            {
                albumHTML = GetHTMLFromURL(sUrl).HTML;
            }
            catch (Exception e)
            {
                string errorMessage = $"Failed to open {sUrl} ({e.Message})";
                Program.MainForm.Log(errorMessage);
#if DEBUG
                Debug.WriteLine(errorMessage);
#endif
                return;
            }

           
            var parser = new HtmlParser();
            var albumHtmlDocument = parser.ParseDocument(albumHTML);
            Program.MainForm.Log($"\r\nParsing {sUrl}");
            var songNodes = albumHtmlDocument.All.Where(element =>
                element.LocalName == "td" &&
                element.ClassName ==
                "playlistDownloadSong"); //.SelectNodes("//td[contains(@class, 'playlistDownloadSong')]");
            var qualityNode = albumHtmlDocument.All.FirstOrDefault(element =>
                element.LocalName == "tr" &&
                element.Id ==
                "songlist_header"); //DocumentNode.SelectNodes("//tr[contains(@id, 'songlist_header')]/th");
            var albumNameNode = albumHtmlDocument.All
                .FirstOrDefault(element => element.Id == "pageContent")
                ?.Children[1]; //DocumentNode.SelectSingleNode("//*[@id=\"EchoTopic\"]/h2[1]");
            var albumImageNodes = albumHtmlDocument.All.Where(element =>
                element.LocalName == "div" &&
                element.ClassName ==
                "albumImage"); //.SelectNodes("//td[contains(@class, 'playlistDownloadSong')]");

            string szAlbumName;
            if (albumNameNode != null && songNodes.Any() && qualityNode != null)
            {
                //Trim spaces and dots!
                szAlbumName = string
                    .Join("_", WebUtility.HtmlDecode(albumNameNode.Text()).Split(Path.GetInvalidFileNameChars()))
                    .Trim(new[] { ' ', '.' });
                //'info' hack. Remove the last words from inner child
                if (albumNameNode.Children != null && albumNameNode.Children.Length > 0 &&
                    szAlbumName.Length >= albumNameNode.Children[0].Text().Length)
                {
                    szAlbumName = szAlbumName.Substring(0, szAlbumName.Length - albumNameNode.Children[0].Text().Length);
                }

            }
            else
            {
                Program.MainForm.Log($"Failed to parse {sUrl}");
                return;
            }

            List<string> albumImageLinks = albumImageNodes
                .Where(albumImageNode => albumImageNode.ChildElementCount > 0)
                .Select(albumImageNode => albumImageNode.Children[0].GetAttribute("href"))
                .ToList();

            //byte[] ArtFix = null;
            //string ArtFixPath = "";
            //if (m_bArtFix)
            //{
            //    //find and download best image
            //    string bestLink = albumImageLinks
            //        .FirstOrDefault(link =>
            //        {
            //            string fileName = link.Split('/').Last();
            //            string fileExtension = fileName.Split('.').Last();
            //            return fileExtension.ToLower().Contains("cover");
            //        }) ?? albumImageLinks.FirstOrDefault();
            //    if (bestLink != null)
            //    {
            //        WebClient downloadClient = new WebClient() { Proxy = null };
            //        ArtFix = downloadClient.DownloadData(bestLink);
            //        ArtFixPath = bestLink;
            //    }
            //}
            if (m_bDownloadArt)
            {
            
                if (albumImageLinks.Count != 0)
                {
                    Directory.CreateDirectory(m_szDownloadPath + "\\" + szAlbumName + "\\art\\");
                    foreach (var albumImageLink in albumImageLinks)
                    {
                        WebClient downloadClient = new WebClient() { Proxy = null };

                        string filename = albumImageLink.Split('/').Last();
                        var shh = string
                            .Join("_", Uri.UnescapeDataString(filename).Split(Path.GetInvalidFileNameChars()))
                            .Trim(new[] { ' ', '.' });
                        Task currentTask = downloadClient.DownloadFileTaskAsync(new Uri(albumImageLink), m_szDownloadPath + "\\" + szAlbumName + "\\art\\" + shh);
                        currentTask.ContinueWith(
                            task => { downloadClient.Dispose(); });
                        currentTasks.Add(currentTask);

                    }
                }
            }


            Directory.CreateDirectory(Downloader.m_szDownloadPath + "\\" + szAlbumName);
            var selectedFormat = ".mp3";
            if (eQuality != EDownloadQuality.QUALITY_MP3_ONLY)
            {
                //List<IElement> qualitySubNodes = qualityNodes.Skip(3).ToList();
                //find non mp3 file
                bool bBreak = false;
                foreach (var cell in qualityNode.Children.ToList())
                {
                    foreach (var cellchilds in cell.Children.ToList())
                    {
                        if (cellchilds.InnerHtml == "FLAC")
                        {
                            selectedFormat = ".flac";
                            bBreak = true;
                            break;
                        }

                        if (cellchilds.InnerHtml == "OGG")
                        {
                            selectedFormat = ".ogg";
                            bBreak = true;
                            break;
                        }

                        if (cellchilds.InnerHtml == "M4A")
                        {
                            selectedFormat = ".m4a";
                            bBreak = true;
                            break;
                        }
                    }

                    if (bBreak)
                    {
                        break;
                    }
                }
            }

            int nTotalSongs = songNodes.Count();
            int nCurrentSong = 0;
            try
            {
                Parallel.ForEach(songNodes, g_songsParralelOptions, song =>
                {
                    var songPageURL =
                        m_szHostName + song.Children[0].GetAttribute("href"); //["href"].Value;

                    IHtmlDocument songPageDocument;

                    try
                    {
                        if (g_songsParralelOptions.MaxDegreeOfParallelism == 1)
                        {
                            songPageDocument = new HtmlParser().ParseDocument(GetHTMLFromURL(songPageURL).HTML);
                        }
                        else
                        {
                            songPageDocument = parser.ParseDocument(GetHTMLFromURL(songPageURL).HTML);
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
                        songPageDocument.All.Where(element => element.ClassName == "songDownloadLink")
                            .ToList(); //"//span[@class='songDownloadLink']"); //[1].ParentElement.GetAttribute("href");
                    //Do not use Parallel.ForEach as we usually have ~2 nodes
                    int nDownloadNodes = downloadLinkNodes.Count();
                    for (var index = 0; index < nDownloadNodes; index++)
                    {
                        var dlsongentry = downloadLinkNodes[index];
                        var songFileURL = dlsongentry.ParentElement.GetAttribute("href"); //.Value;
                        if (bIsPlaylist)
                        {
                            if (songFileURL.EndsWith(".mp3") && eQuality != EDownloadQuality.QUALITY_MP3_ONLY && nDownloadNodes > 1)
                            {
                                continue;
                            }
                        }
                        if (songFileURL.EndsWith(selectedFormat) || bIsPlaylist)
                        {
                            var name = WebUtility.UrlDecode(
                                songFileURL.Substring(songFileURL.LastIndexOf("/", StringComparison.Ordinal) + 1));

                            if (!IsDownloading)
                            {
                                return;
                            }

                            if (!m_bSuppessLogs)
                            {
                                Program.MainForm.Log($"Downloading {name}...");
                            }

                            string filename = m_szDownloadPath + "\\" + szAlbumName + "\\" +
                                              string.Join("_", name.Split(Path.GetInvalidFileNameChars()));

                            if(Downloader.m_bSkipDownloaded && File.Exists(filename))
                            {
                                ++nCurrentSong;
                                if (nTotalAlbums == 1)
                                {
                                    UpdateTitle(nCurrentSong, nTotalSongs);
                                }
                                return;
                            }

                            try
                            {
                                WebClient downloadClient = new WebClient() {Proxy = null};
                                cancelTokenSource.Token.Register(downloadClient.CancelAsync);

                                Task currentTask = downloadClient.DownloadFileTaskAsync(new Uri(songFileURL), filename);
                                currentTask.ContinueWith(
                                    task =>
                                    {
                                        downloadClient.Dispose();

                                        if (!IsDownloading)
                                        {
                                            File.Delete(filename);
                                            return;
                                        }

                                        //if (m_bArtFix)
                                        //{
                                        //    var file = AudioFile.Create(filename);
                                        //    if (file.Tag.Pictures.Length > 0)
                                        //    {
                                        //        if (file.Tag.Pictures.All(n => n.Type != PictureType.FrontCover))
                                        //        {
                                        //            file.Tag.Pictures[0].Type = PictureType.FrontCover;
                                        //        }
                                        //    }
                                        //    else
                                        //    {
                                        //        file.Tag.Pictures = new IPicture[]
                                        //        {
                                        //            new Picture(new ByteVector(ArtFix))
                                        //            {
                                        //                Type = PictureType.FrontCover,
                                        //                Description = "Cover",
                                        //                MimeType = MimeMapping.GetMimeMapping(ArtFixPath)
                                        //            }
                                        //        };
                                        //    }
                                        //    file.Save();
                                        //}

                                        if (!m_bSuppessLogs && !cancelTokenSource.Token.IsCancellationRequested)
                                        {
                                            Program.MainForm.Log($"{name} has been downloaded!");
                                        }

                                        ++nCurrentSong;
                                        if (nTotalAlbums == 1)
                                        {
                                            UpdateTitle(nCurrentSong, nTotalSongs);
                                        }
                                        
                                    }, TaskContinuationOptions.ExecuteSynchronously);
                                currentTasks.Add(currentTask);
                            }
                            catch (Exception e)
                            {
                                string errorMessage = $"Failed to download {songFileURL} to {filename}\r\n({e.Message})";
                                Program.MainForm.Log(errorMessage);
#if DEBUG
                                Debug.WriteLine(errorMessage);
#endif

                                try
                                {
                                    if (File.Exists(filename))
                                    {
                                        File.Delete(filename);
                                    }
                                }
                                catch (Exception removeexc)
                                {
                                    Program.MainForm.Log($"Failed to remove semi-downloaded {filename}\r\n({removeexc.Message})");
                                }
                            }
                        }
                    }
                });
                Task.WaitAll(currentTasks.ToArray(), cancelTokenSource.Token);
            }
            catch (OperationCanceledException)
            {

            }


            Program.MainForm.Log($"Finished downloading {szAlbumName}!");
            if (nTotalAlbums != 1)
            {
                UpdateTitle(++nAlbumsDownloaded, nTotalAlbums);
            }
        }
    }

    //https://stackoverflow.com/a/24187171
    public static class TaskbarProgress
    {
        public enum TaskbarStates
        {
            NoProgress = 0,
            Indeterminate = 0x1,
            Normal = 0x2,
            Error = 0x4,
            Paused = 0x8
        }

        [ComImport()]
        [Guid("ea1afb91-9e28-4b86-90e9-9e9f8a5eefaf")]
        [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        private interface ITaskbarList3
        {
            // ITaskbarList
            [PreserveSig]
            void HrInit();

            [PreserveSig]
            void AddTab(IntPtr hwnd);

            [PreserveSig]
            void DeleteTab(IntPtr hwnd);

            [PreserveSig]
            void ActivateTab(IntPtr hwnd);

            [PreserveSig]
            void SetActiveAlt(IntPtr hwnd);

            // ITaskbarList2
            [PreserveSig]
            void MarkFullscreenWindow(IntPtr hwnd, [MarshalAs(UnmanagedType.Bool)] bool fFullscreen);

            // ITaskbarList3
            [PreserveSig]
            void SetProgressValue(IntPtr hwnd, UInt64 ullCompleted, UInt64 ullTotal);

            [PreserveSig]
            void SetProgressState(IntPtr hwnd, TaskbarStates state);
        }

        [ComImport()]
        [Guid("56fdf344-fd6d-11d0-958a-006097c9a090")]
        [ClassInterface(ClassInterfaceType.None)]
        private class TaskbarInstance
        {
        }

        private static readonly ITaskbarList3 taskbarInstance = (ITaskbarList3)new TaskbarInstance();
        private static readonly bool taskbarSupported = Environment.OSVersion.Version >= new Version(6, 1);

        public static void SetState(IntPtr windowHandle, TaskbarStates taskbarState)
        {
            if (taskbarSupported) taskbarInstance.SetProgressState(windowHandle, taskbarState);
        }

        public static void SetValue(IntPtr windowHandle, double progressValue, double progressMax)
        {
            if (taskbarSupported)
                taskbarInstance.SetProgressValue(windowHandle, (ulong)progressValue, (ulong)progressMax);
        }
    }
}
