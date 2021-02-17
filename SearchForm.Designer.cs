namespace KhinsiderDownloader
{
	partial class SearchForm
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SearchForm));
			this.btn_search = new System.Windows.Forms.Button();
			this.txt_input = new System.Windows.Forms.TextBox();
			this.pic_album = new System.Windows.Forms.PictureBox();
			this.lbl_noimage = new System.Windows.Forms.Label();
			this.btn_add = new System.Windows.Forms.Button();
			this.btn_add_all = new System.Windows.Forms.Button();
			this.list_result = new System.Windows.Forms.ListBox();
			((System.ComponentModel.ISupportInitialize)(this.pic_album)).BeginInit();
			this.SuspendLayout();
			// 
			// btn_search
			// 
			this.btn_search.Location = new System.Drawing.Point(12, 38);
			this.btn_search.Name = "btn_search";
			this.btn_search.Size = new System.Drawing.Size(254, 23);
			this.btn_search.TabIndex = 0;
			this.btn_search.Text = "Search";
			this.btn_search.UseVisualStyleBackColor = true;
			this.btn_search.Click += new System.EventHandler(this.btn_search_Click);
			// 
			// txt_input
			// 
			this.txt_input.Location = new System.Drawing.Point(12, 12);
			this.txt_input.Name = "txt_input";
			this.txt_input.Size = new System.Drawing.Size(254, 20);
			this.txt_input.TabIndex = 1;
			// 
			// pic_album
			// 
			this.pic_album.Location = new System.Drawing.Point(272, 11);
			this.pic_album.Name = "pic_album";
			this.pic_album.Size = new System.Drawing.Size(170, 152);
			this.pic_album.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
			this.pic_album.TabIndex = 2;
			this.pic_album.TabStop = false;
			// 
			// lbl_noimage
			// 
			this.lbl_noimage.AutoSize = true;
			this.lbl_noimage.Location = new System.Drawing.Point(330, 81);
			this.lbl_noimage.Name = "lbl_noimage";
			this.lbl_noimage.Size = new System.Drawing.Size(53, 13);
			this.lbl_noimage.TabIndex = 3;
			this.lbl_noimage.Text = "No Image";
			// 
			// btn_add
			// 
			this.btn_add.Location = new System.Drawing.Point(272, 169);
			this.btn_add.Name = "btn_add";
			this.btn_add.Size = new System.Drawing.Size(170, 23);
			this.btn_add.TabIndex = 4;
			this.btn_add.Text = "Add";
			this.btn_add.UseVisualStyleBackColor = true;
			this.btn_add.Click += new System.EventHandler(this.btn_add_Click);
			// 
			// btn_add_all
			// 
			this.btn_add_all.Location = new System.Drawing.Point(272, 204);
			this.btn_add_all.Name = "btn_add_all";
			this.btn_add_all.Size = new System.Drawing.Size(170, 23);
			this.btn_add_all.TabIndex = 5;
			this.btn_add_all.Text = "Add All";
			this.btn_add_all.UseVisualStyleBackColor = true;
			this.btn_add_all.Click += new System.EventHandler(this.btn_add_all_Click);
			// 
			// list_result
			// 
			this.list_result.FormattingEnabled = true;
			this.list_result.Location = new System.Drawing.Point(12, 67);
			this.list_result.Name = "list_result";
			this.list_result.Size = new System.Drawing.Size(254, 160);
			this.list_result.TabIndex = 6;
			this.list_result.SelectedIndexChanged += new System.EventHandler(this.list_result_SelectedIndexChanged);
			// 
			// SearchForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(453, 237);
			this.Controls.Add(this.list_result);
			this.Controls.Add(this.btn_add_all);
			this.Controls.Add(this.btn_add);
			this.Controls.Add(this.lbl_noimage);
			this.Controls.Add(this.pic_album);
			this.Controls.Add(this.txt_input);
			this.Controls.Add(this.btn_search);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "SearchForm";
			this.Text = "Search";
			((System.ComponentModel.ISupportInitialize)(this.pic_album)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button btn_search;
		private System.Windows.Forms.TextBox txt_input;
		private System.Windows.Forms.PictureBox pic_album;
		private System.Windows.Forms.Label lbl_noimage;
		private System.Windows.Forms.Button btn_add;
		private System.Windows.Forms.Button btn_add_all;
		private System.Windows.Forms.ListBox list_result;
	}
}