using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Threading.Tasks;
using System.Web;
using System.Windows.Forms;
using AngleSharp.Dom;
using AngleSharp.Html.Parser;
using KhinsiderDownloader.Properties;


namespace KhinsiderDownloader
{
	public partial class SearchForm : Form
	{
		static string urlPrefix = "https://downloads.khinsider.com";

		WebClient webClient;
		public TextBox linkbox = null;
		public SearchForm()
		{
			InitializeComponent();
			webClient = new WebClient();
			pic_album.Image = Resources.no_image;
		}


		class SearchItem
		{
			public override string ToString()
			{
				return Name;
			}
			public string Name;
			public string Url;
			public Bitmap CoverImage;
			public bool ImageResolved = false;
		}
		
		List<SearchItem> DoSearch(string query)
		{
			this.Invoke(new Action(() => { this.Cursor = Cursors.WaitCursor; }));

			List<SearchItem> searchResult = new List<SearchItem>();
			var parser = new HtmlParser();
			var endpointURL = "https://downloads.khinsider.com/search?search=" + query + "&albumListSize=compact";
			Downloader.HTMLResult downloadHtmlResult = Downloader.GetHTMLFromURL(endpointURL);
			var htmlDocument = parser.ParseDocument(downloadHtmlResult.HTML);
			//Get Album name
			var albumNameNode = htmlDocument.All.FirstOrDefault(element => element.LocalName == "div" && element.Id == "pageContent");
			if (albumNameNode == null)
			{
				return searchResult;
			}
			var searchresult = albumNameNode.Children[0].InnerHtml;
			if (searchresult != "Search")
			{
				SearchItem searchItem = new SearchItem();
				searchItem.Name = HttpUtility.HtmlDecode(albumNameNode.Children[1].InnerHtml);
				searchItem.Url = downloadHtmlResult.ResponseURI;
				searchResult.Add(searchItem);
				this.Invoke(new Action(() => { this.Cursor = Cursors.Default; }));
				return searchResult;
			}
			else
            {
                var albumlist = albumNameNode.Children[3];
                if (albumlist.ChildElementCount == 0)
                {
                    return searchResult;
                }
                var searchresults = albumlist.Children[1].Children[0].Children[0].Children.Skip(1);//Where(element=>element.LocalName == "a");
				foreach (var node in searchresults)
                {
                    var cellnode = node.Children[1].Children[0];
					SearchItem searchItem = new SearchItem();
					searchItem.Name = HttpUtility.HtmlDecode(cellnode.InnerHtml);
					searchItem.Url = cellnode.Attributes["href"].Value;
					searchResult.Add(searchItem);
				}
			}
		
			return searchResult;
			
		}

		void SearchStub()
		{
			Task.Run(() =>
			{
				
				pic_album.Invoke(new Action(() => { pic_album.Image = null; }));
				pic_album.Image = null;
				var result = DoSearch(txt_input.Text);
                this.Invoke(new Action(() => { this.Cursor = Cursors.Default; }));
				list_result.Invoke(new Action(() => { list_result.Items.Clear(); }));
				foreach (var item in result)
				{
					list_result.Invoke(new Action(() => { list_result.Items.Add(item); }));
				}
				
			});

		}
		private void btn_search_Click(object sender, EventArgs e)
		{
			SearchStub();
		}

		void ResolveImage(ref SearchItem searchItem)
		{
			
			searchItem.ImageResolved = true;
			try
			{

				var parser = new HtmlParser();
                var htmlPage = Downloader.GetHTMLFromURL(urlPrefix + searchItem.Url).HTML;
				var htmlDocument = parser.ParseDocument(htmlPage);
				var albumNameNode = htmlDocument.All.FirstOrDefault(element => element.ClassName == "albumImage");
                var imagenode = albumNameNode.Children[0];
				if (imagenode != null)
				{
					var imageURL = imagenode.GetAttribute("href");
					using (MemoryStream stream = new MemoryStream(webClient.DownloadData(imageURL)))
					{
						searchItem.CoverImage = new Bitmap(Image.FromStream(stream));
					}
				}

				ShowImage(searchItem.CoverImage);
			}
			catch (Exception)
			{
			}
		}

		void ShowImage(Bitmap bitmap)
		{
			if (bitmap == null)
			{
				pic_album.Invoke(new Action(() => { pic_album.Image = Properties.Resources.no_image; }));
			}
			else
			{
				pic_album.Invoke(new Action(() => { pic_album.Image = bitmap; }));

			}
		}
		private void list_result_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (sender == null)
			{
				return;
			}

			SearchItem currentItem = (SearchItem) list_result.SelectedItem;
			if (currentItem == null)
			{
				return;
			}
			if (!currentItem.ImageResolved)
			{
				Task.Run(() => ResolveImage(ref currentItem));
			}
			else
			{
				ShowImage(currentItem.CoverImage);
			}
		}

		private void btn_add_Click(object sender, EventArgs e)
		{
			if (list_result.SelectedItem == null)
			{
				return;
			}
			SearchItem currentItem = (SearchItem)list_result.SelectedItem;
			linkbox.Text += urlPrefix + currentItem.Url + Environment.NewLine;
		}

		private void btn_add_all_Click(object sender, EventArgs e)
		{
			foreach (var item in list_result.Items)
			{
				SearchItem currentItem = (SearchItem)item;
				linkbox.Text += urlPrefix + currentItem.Url + Environment.NewLine;
			}
		}

        private void txt_input_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
				SearchStub();
            }
        }
    }
}