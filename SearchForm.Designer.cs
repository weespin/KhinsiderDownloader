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
            this.btn_add = new System.Windows.Forms.Button();
            this.btn_add_all = new System.Windows.Forms.Button();
            this.list_result = new System.Windows.Forms.ListBox();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            ((System.ComponentModel.ISupportInitialize)(this.pic_album)).BeginInit();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // btn_search
            // 
            this.btn_search.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btn_search.Location = new System.Drawing.Point(3, 34);
            this.btn_search.Name = "btn_search";
            this.btn_search.Size = new System.Drawing.Size(291, 25);
            this.btn_search.TabIndex = 0;
            this.btn_search.Text = "Search";
            this.btn_search.UseVisualStyleBackColor = true;
            this.btn_search.Click += new System.EventHandler(this.btn_search_Click);
            // 
            // txt_input
            // 
            this.txt_input.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txt_input.Location = new System.Drawing.Point(3, 3);
            this.txt_input.Name = "txt_input";
            this.txt_input.Size = new System.Drawing.Size(291, 20);
            this.txt_input.TabIndex = 1;
            this.txt_input.Text = "Search...";
            this.txt_input.Enter += new System.EventHandler(this.txt_input_Enter);
            this.txt_input.KeyDown += new System.Windows.Forms.KeyEventHandler(this.txt_input_KeyDown);
            this.txt_input.Leave += new System.EventHandler(this.txt_input_Leave);
            // 
            // pic_album
            // 
            this.pic_album.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pic_album.Location = new System.Drawing.Point(3, 3);
            this.pic_album.Name = "pic_album";
            this.pic_album.Size = new System.Drawing.Size(237, 243);
            this.pic_album.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pic_album.TabIndex = 2;
            this.pic_album.TabStop = false;
            // 
            // btn_add
            // 
            this.btn_add.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btn_add.Location = new System.Drawing.Point(3, 252);
            this.btn_add.Name = "btn_add";
            this.btn_add.Size = new System.Drawing.Size(237, 25);
            this.btn_add.TabIndex = 4;
            this.btn_add.Text = "Add";
            this.btn_add.UseVisualStyleBackColor = true;
            this.btn_add.Click += new System.EventHandler(this.btn_add_Click);
            // 
            // btn_add_all
            // 
            this.btn_add_all.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btn_add_all.Location = new System.Drawing.Point(3, 283);
            this.btn_add_all.Name = "btn_add_all";
            this.btn_add_all.Size = new System.Drawing.Size(237, 26);
            this.btn_add_all.TabIndex = 5;
            this.btn_add_all.Text = "Add All";
            this.btn_add_all.UseVisualStyleBackColor = true;
            this.btn_add_all.Click += new System.EventHandler(this.btn_add_all_Click);
            // 
            // list_result
            // 
            this.list_result.Dock = System.Windows.Forms.DockStyle.Fill;
            this.list_result.FormattingEnabled = true;
            this.list_result.Location = new System.Drawing.Point(3, 65);
            this.list_result.Name = "list_result";
            this.list_result.Size = new System.Drawing.Size(291, 244);
            this.list_result.TabIndex = 6;
            this.list_result.SelectedIndexChanged += new System.EventHandler(this.list_result_SelectedIndexChanged);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 55F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 45F));
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel3, 1, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(552, 318);
            this.tableLayoutPanel1.TabIndex = 7;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 1;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Controls.Add(this.txt_input, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.list_result, 0, 2);
            this.tableLayoutPanel2.Controls.Add(this.btn_search, 0, 1);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 3;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 80F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(297, 312);
            this.tableLayoutPanel2.TabIndex = 0;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 1;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Controls.Add(this.pic_album, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.btn_add_all, 0, 2);
            this.tableLayoutPanel3.Controls.Add(this.btn_add, 0, 1);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(306, 3);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 3;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 80F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(243, 312);
            this.tableLayoutPanel3.TabIndex = 1;
            // 
            // SearchForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(552, 318);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "SearchForm";
            this.Text = "Search";
            ((System.ComponentModel.ISupportInitialize)(this.pic_album)).EndInit();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.tableLayoutPanel3.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button btn_search;
		private System.Windows.Forms.TextBox txt_input;
		private System.Windows.Forms.PictureBox pic_album;
		private System.Windows.Forms.Button btn_add;
		private System.Windows.Forms.Button btn_add_all;
		private System.Windows.Forms.ListBox list_result;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
	}
}