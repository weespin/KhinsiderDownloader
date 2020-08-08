namespace KhinsiderDownloader
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
			this.urllist_box = new System.Windows.Forms.TextBox();
			this.textBox2 = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.radioButton2 = new System.Windows.Forms.RadioButton();
			this.radioButton1 = new System.Windows.Forms.RadioButton();
			this.downloadPath_label = new System.Windows.Forms.Label();
			this.btn_DownloadPath = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// urllist_box
			// 
			this.urllist_box.Location = new System.Drawing.Point(3, 12);
			this.urllist_box.Multiline = true;
			this.urllist_box.Name = "urllist_box";
			this.urllist_box.Size = new System.Drawing.Size(752, 127);
			this.urllist_box.TabIndex = 0;
			// 
			// textBox2
			// 
			this.textBox2.BackColor = System.Drawing.SystemColors.Control;
			this.textBox2.Location = new System.Drawing.Point(3, 145);
			this.textBox2.Multiline = true;
			this.textBox2.Name = "textBox2";
			this.textBox2.ReadOnly = true;
			this.textBox2.Size = new System.Drawing.Size(600, 98);
			this.textBox2.TabIndex = 1;
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(615, 220);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(130, 23);
			this.button1.TabIndex = 2;
			this.button1.Text = "Download";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.radioButton2);
			this.groupBox1.Controls.Add(this.radioButton1);
			this.groupBox1.Location = new System.Drawing.Point(609, 145);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(146, 69);
			this.groupBox1.TabIndex = 3;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Audio Quality";
			// 
			// radioButton2
			// 
			this.radioButton2.AutoSize = true;
			this.radioButton2.Location = new System.Drawing.Point(6, 43);
			this.radioButton2.Name = "radioButton2";
			this.radioButton2.Size = new System.Drawing.Size(137, 17);
			this.radioButton2.TabIndex = 5;
			this.radioButton2.Text = "Best (FLAC/M4A/OGG)";
			this.radioButton2.UseVisualStyleBackColor = true;
			// 
			// radioButton1
			// 
			this.radioButton1.AutoSize = true;
			this.radioButton1.Checked = true;
			this.radioButton1.Location = new System.Drawing.Point(6, 19);
			this.radioButton1.Name = "radioButton1";
			this.radioButton1.Size = new System.Drawing.Size(71, 17);
			this.radioButton1.TabIndex = 4;
			this.radioButton1.TabStop = true;
			this.radioButton1.Text = "MP3 Only";
			this.radioButton1.UseVisualStyleBackColor = true;
			this.radioButton1.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
			// 
			// downloadPath_label
			// 
			this.downloadPath_label.AutoSize = true;
			this.downloadPath_label.Location = new System.Drawing.Point(9, 255);
			this.downloadPath_label.Name = "downloadPath_label";
			this.downloadPath_label.Size = new System.Drawing.Size(35, 13);
			this.downloadPath_label.TabIndex = 4;
			this.downloadPath_label.Text = "label1";
			// 
			// btn_DownloadPath
			// 
			this.btn_DownloadPath.Location = new System.Drawing.Point(649, 255);
			this.btn_DownloadPath.Name = "btn_DownloadPath";
			this.btn_DownloadPath.Size = new System.Drawing.Size(96, 23);
			this.btn_DownloadPath.TabIndex = 5;
			this.btn_DownloadPath.Text = "Select Path...";
			this.btn_DownloadPath.UseVisualStyleBackColor = true;
			this.btn_DownloadPath.Click += new System.EventHandler(this.btn_DownloadPath_Click);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.ForeColor = System.Drawing.Color.DarkGray;
			this.label1.Location = new System.Drawing.Point(675, 288);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(82, 13);
			this.label1.TabIndex = 6;
			this.label1.Text = "Weespin - 2020";
			this.label1.DoubleClick += new System.EventHandler(this.label1_DoubleClick);
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(757, 304);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.btn_DownloadPath);
			this.Controls.Add(this.downloadPath_label);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.textBox2);
			this.Controls.Add(this.urllist_box);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "Form1";
			this.Text = "KhinsiderDownloader";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox urllist_box;
		private System.Windows.Forms.TextBox textBox2;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton radioButton2;
		private System.Windows.Forms.RadioButton radioButton1;
		private System.Windows.Forms.Label downloadPath_label;
		private System.Windows.Forms.Button btn_DownloadPath;
		private System.Windows.Forms.Label label1;
	}
}

