﻿namespace KhinsiderDownloader
{
	partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.txt_urllist = new System.Windows.Forms.TextBox();
            this.txt_log = new System.Windows.Forms.TextBox();
            this.btn_download = new System.Windows.Forms.Button();
            this.box_quality = new System.Windows.Forms.GroupBox();
            this.radio_betterquality = new System.Windows.Forms.RadioButton();
            this.radio_mp3only = new System.Windows.Forms.RadioButton();
            this.lbl_path = new System.Windows.Forms.Label();
            this.btn_selectpath = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.num_threads = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.num_album_threads = new System.Windows.Forms.NumericUpDown();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.chk_suppress_downloading_logs = new System.Windows.Forms.CheckBox();
            this.btn_opensearch = new System.Windows.Forms.Button();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.box_quality.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.num_threads)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.num_album_threads)).BeginInit();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.tableLayoutPanel5.SuspendLayout();
            this.SuspendLayout();
            // 
            // txt_urllist
            // 
            this.txt_urllist.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txt_urllist.Location = new System.Drawing.Point(3, 3);
            this.txt_urllist.Multiline = true;
            this.txt_urllist.Name = "txt_urllist";
            this.txt_urllist.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txt_urllist.Size = new System.Drawing.Size(650, 142);
            this.txt_urllist.TabIndex = 0;
            // 
            // txt_log
            // 
            this.txt_log.BackColor = System.Drawing.SystemColors.Control;
            this.txt_log.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txt_log.Location = new System.Drawing.Point(3, 3);
            this.txt_log.Multiline = true;
            this.txt_log.Name = "txt_log";
            this.txt_log.ReadOnly = true;
            this.txt_log.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txt_log.Size = new System.Drawing.Size(487, 136);
            this.txt_log.TabIndex = 1;
            // 
            // btn_download
            // 
            this.btn_download.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btn_download.Location = new System.Drawing.Point(3, 99);
            this.btn_download.Name = "btn_download";
            this.btn_download.Size = new System.Drawing.Size(145, 34);
            this.btn_download.TabIndex = 2;
            this.btn_download.Text = "Download";
            this.btn_download.UseVisualStyleBackColor = true;
            this.btn_download.Click += new System.EventHandler(this.button1_Click);
            // 
            // box_quality
            // 
            this.box_quality.Controls.Add(this.radio_betterquality);
            this.box_quality.Controls.Add(this.radio_mp3only);
            this.box_quality.Dock = System.Windows.Forms.DockStyle.Fill;
            this.box_quality.Location = new System.Drawing.Point(3, 3);
            this.box_quality.Name = "box_quality";
            this.box_quality.Size = new System.Drawing.Size(145, 90);
            this.box_quality.TabIndex = 3;
            this.box_quality.TabStop = false;
            this.box_quality.Text = "Audio Quality";
            // 
            // radio_betterquality
            // 
            this.radio_betterquality.AutoSize = true;
            this.radio_betterquality.Location = new System.Drawing.Point(6, 43);
            this.radio_betterquality.Name = "radio_betterquality";
            this.radio_betterquality.Size = new System.Drawing.Size(137, 17);
            this.radio_betterquality.TabIndex = 5;
            this.radio_betterquality.Text = "Best (FLAC/M4A/OGG)";
            this.radio_betterquality.UseVisualStyleBackColor = true;
            // 
            // radio_mp3only
            // 
            this.radio_mp3only.AutoSize = true;
            this.radio_mp3only.Checked = true;
            this.radio_mp3only.Location = new System.Drawing.Point(6, 19);
            this.radio_mp3only.Name = "radio_mp3only";
            this.radio_mp3only.Size = new System.Drawing.Size(71, 17);
            this.radio_mp3only.TabIndex = 4;
            this.radio_mp3only.TabStop = true;
            this.radio_mp3only.Text = "MP3 Only";
            this.radio_mp3only.UseVisualStyleBackColor = true;
            this.radio_mp3only.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
            // 
            // lbl_path
            // 
            this.lbl_path.AutoSize = true;
            this.lbl_path.Location = new System.Drawing.Point(3, 0);
            this.lbl_path.Name = "lbl_path";
            this.lbl_path.Size = new System.Drawing.Size(35, 13);
            this.lbl_path.TabIndex = 4;
            this.lbl_path.Text = "label1";
            // 
            // btn_selectpath
            // 
            this.btn_selectpath.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.btn_selectpath.Location = new System.Drawing.Point(420, 3);
            this.btn_selectpath.Name = "btn_selectpath";
            this.btn_selectpath.Size = new System.Drawing.Size(88, 22);
            this.btn_selectpath.TabIndex = 5;
            this.btn_selectpath.Text = "Select Path...";
            this.btn_selectpath.UseVisualStyleBackColor = true;
            this.btn_selectpath.Click += new System.EventHandler(this.btn_DownloadPath_Click);
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.DarkGray;
            this.label1.Location = new System.Drawing.Point(565, 2);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(82, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Weespin - 2022";
            this.label1.DoubleClick += new System.EventHandler(this.label1_DoubleClick);
            // 
            // num_threads
            // 
            this.num_threads.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.num_threads.Location = new System.Drawing.Point(355, 4);
            this.num_threads.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.num_threads.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.num_threads.Name = "num_threads";
            this.num_threads.Size = new System.Drawing.Size(59, 20);
            this.num_threads.TabIndex = 8;
            this.num_threads.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.num_threads.ValueChanged += new System.EventHandler(this.num_threads_ValueChanged);
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(248, 7);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(99, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "Threads per Album:";
            // 
            // label3
            // 
            this.label3.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(97, 7);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(81, 13);
            this.label3.TabIndex = 11;
            this.label3.Text = "Album Threads:";
            // 
            // num_album_threads
            // 
            this.num_album_threads.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.num_album_threads.Location = new System.Drawing.Point(187, 4);
            this.num_album_threads.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.num_album_threads.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.num_album_threads.Name = "num_album_threads";
            this.num_album_threads.Size = new System.Drawing.Size(52, 20);
            this.num_album_threads.TabIndex = 10;
            this.num_album_threads.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.num_album_threads.ValueChanged += new System.EventHandler(this.num_album_threads_ValueChanged);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50.5F));
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.txt_urllist, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel4, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel5, 0, 3);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 4;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 34F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 22F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(656, 353);
            this.tableLayoutPanel1.TabIndex = 13;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 2;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20.79566F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 156F));
            this.tableLayoutPanel2.Controls.Add(this.txt_log, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel3, 1, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 151);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(650, 142);
            this.tableLayoutPanel2.TabIndex = 1;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 1;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel3.Controls.Add(this.box_quality, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.btn_download, 0, 1);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(496, 3);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 2;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 70.7317F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 29.26829F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(151, 136);
            this.tableLayoutPanel3.TabIndex = 2;
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 7;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 90F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 109F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 65F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 94F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 139F));
            this.tableLayoutPanel4.Controls.Add(this.chk_suppress_downloading_logs, 0, 0);
            this.tableLayoutPanel4.Controls.Add(this.btn_opensearch, 6, 0);
            this.tableLayoutPanel4.Controls.Add(this.num_threads, 4, 0);
            this.tableLayoutPanel4.Controls.Add(this.label2, 3, 0);
            this.tableLayoutPanel4.Controls.Add(this.btn_selectpath, 5, 0);
            this.tableLayoutPanel4.Controls.Add(this.num_album_threads, 2, 0);
            this.tableLayoutPanel4.Controls.Add(this.label3, 1, 0);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(3, 299);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 1;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(650, 28);
            this.tableLayoutPanel4.TabIndex = 2;
            // 
            // chk_suppress_downloading_logs
            // 
            this.chk_suppress_downloading_logs.AutoSize = true;
            this.chk_suppress_downloading_logs.Location = new System.Drawing.Point(3, 3);
            this.chk_suppress_downloading_logs.Name = "chk_suppress_downloading_logs";
            this.chk_suppress_downloading_logs.Size = new System.Drawing.Size(87, 17);
            this.chk_suppress_downloading_logs.TabIndex = 12;
            this.chk_suppress_downloading_logs.Text = "Suppress Logs";
            this.chk_suppress_downloading_logs.UseVisualStyleBackColor = true;
            this.chk_suppress_downloading_logs.CheckedChanged += new System.EventHandler(this.chk_suppress_downloading_logs_CheckedChanged);
            // 
            // btn_opensearch
            // 
            this.btn_opensearch.Location = new System.Drawing.Point(514, 3);
            this.btn_opensearch.Name = "btn_opensearch";
            this.btn_opensearch.Size = new System.Drawing.Size(130, 22);
            this.btn_opensearch.TabIndex = 7;
            this.btn_opensearch.Text = "Search";
            this.btn_opensearch.UseVisualStyleBackColor = true;
            this.btn_opensearch.Click += new System.EventHandler(this.btn_opensearch_Click);
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.ColumnCount = 2;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel5.Controls.Add(this.lbl_path, 0, 0);
            this.tableLayoutPanel5.Controls.Add(this.label1, 1, 0);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(3, 333);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 1;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(650, 17);
            this.tableLayoutPanel5.TabIndex = 3;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(757, 304);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "KhinsiderDownloader";
            this.box_quality.ResumeLayout(false);
            this.box_quality.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.num_threads)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.num_album_threads)).EndInit();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel4.PerformLayout();
            this.tableLayoutPanel5.ResumeLayout(false);
            this.tableLayoutPanel5.PerformLayout();
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.TextBox txt_urllist;
		private System.Windows.Forms.TextBox txt_log;
		private System.Windows.Forms.Button btn_download;
		private System.Windows.Forms.GroupBox box_quality;
		private System.Windows.Forms.RadioButton radio_betterquality;
		private System.Windows.Forms.RadioButton radio_mp3only;
		private System.Windows.Forms.Label lbl_path;
		private System.Windows.Forms.Button btn_selectpath;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.NumericUpDown num_threads;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.NumericUpDown num_album_threads;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.CheckBox chk_suppress_downloading_logs;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private System.Windows.Forms.Button btn_opensearch;
    }
}

