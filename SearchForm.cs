using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Net;
using System.Threading.Tasks;
using System.Windows.Forms;
using HtmlAgilityPack;
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
			
			HtmlWeb webContext = new HtmlWeb();
			
			var htmlDocument = webContext.Load("https://downloads.khinsider.com/search?search=" + query);
			//Get Album name
			var albumname = htmlDocument.DocumentNode.SelectSingleNode("//*[@id=\"EchoTopic\"]/h2[1]").InnerText;
			if (albumname != "Search")
			{
				SearchItem searchItem = new SearchItem();
				searchItem.Name = albumname;
				searchItem.Url = webContext.ResponseUri.ToString();
				searchResult.Add(searchItem);
				this.Invoke(new Action(() => { this.Cursor = Cursors.Default; }));
				return searchResult;
			}
			else
			{
				var searchresults = htmlDocument.DocumentNode.SelectNodes("//*[@id=\"EchoTopic\"]/p[2]/a");
				foreach (var node in searchresults)
				{
					SearchItem searchItem = new SearchItem();
					searchItem.Name = node.InnerText;
					searchItem.Url = node.Attributes["href"].Value;
					searchResult.Add(searchItem);

				}
			}
			this.Invoke(new Action(() => { this.Cursor = Cursors.Default; }));
			return searchResult;
		}

		void SearchStub()
		{
			Task.Run(() =>
			{
				
				pic_album.Invoke(new Action(() => { pic_album.Image = null; }));
				pic_album.Image = null;
				var result = DoSearch(txt_input.Text);
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
				
				HtmlWeb webContext = new HtmlWeb();
				var htmlDocument = webContext.Load(urlPrefix+searchItem.Url);
				var possibleCoverNode = 
					htmlDocument.DocumentNode.SelectSingleNode("/html[1]/body[1]/table[1]/tbody[1]/tr[1]/td[1]/div[1]/div[1]/div[2]/div[2]/table[1]/tbody[1]/tr[1]/td[1]/div[1]/table[1]/tr[1]/td[1]/div[1]/a[1]");
				if (possibleCoverNode != null)
				{
					var imageURL = possibleCoverNode.Attributes["href"].Value;

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
			//lbl_noimage.Invoke(new Action(() => { lbl_noimage.Visible = bitmap == null; }));

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

		private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
		{

		}

		private void SearchForm_Resize(object sender, EventArgs e)
		{
			
		}
	}
}