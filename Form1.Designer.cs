namespace explorer
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            this.iconList = new System.Windows.Forms.ImageList(this.components);
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.btnRefresh = new System.Windows.Forms.Button();
            this.showDLLsBtn = new System.Windows.Forms.Button();
            this.processDataGrid = new System.Windows.Forms.DataGridView();
            this.dllsList = new System.Windows.Forms.ListBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.labelName = new System.Windows.Forms.Label();
            this.labelOwner = new System.Windows.Forms.Label();
            this.labelAttributes = new System.Windows.Forms.Label();
            this.labelCreationTime = new System.Windows.Forms.Label();
            this.labelAccessTime = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.aclList = new System.Windows.Forms.ListBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btnOwnerChange = new System.Windows.Forms.Button();
            this.addAccountBtn = new System.Windows.Forms.Button();
            this.AccountTextBox = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.radioButtonRemove = new System.Windows.Forms.RadioButton();
            this.radioButtonAdd = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radioButtonDeny = new System.Windows.Forms.RadioButton();
            this.radioButtonAllow = new System.Windows.Forms.RadioButton();
            this.btnShowUser = new System.Windows.Forms.Button();
            this.btnShowGroup = new System.Windows.Forms.Button();
            this.accountList = new System.Windows.Forms.ListBox();
            this.btnApply = new System.Windows.Forms.Button();
            this.securityRuleList = new System.Windows.Forms.ListBox();
            this.fileTypeLabel = new System.Windows.Forms.Label();
            this.goButton = new System.Windows.Forms.Button();
            this.filePathTextBox = new System.Windows.Forms.TextBox();
            this.backButton = new System.Windows.Forms.Button();
            this.listView1 = new System.Windows.Forms.ListView();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.processDataGrid)).BeginInit();
            this.tabPage2.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // iconList
            // 
            this.iconList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("iconList.ImageStream")));
            this.iconList.TransparentColor = System.Drawing.Color.Transparent;
            this.iconList.Images.SetKeyName(0, "folder.png");
            this.iconList.Images.SetKeyName(1, "folder2.png");
            this.iconList.Images.SetKeyName(2, "file.png");
            this.iconList.Images.SetKeyName(3, "doc.png");
            this.iconList.Images.SetKeyName(4, "pdf.png");
            this.iconList.Images.SetKeyName(5, "mp3.png");
            this.iconList.Images.SetKeyName(6, "mp4.png");
            this.iconList.Images.SetKeyName(7, "exe.png");
            this.iconList.Images.SetKeyName(8, "unknown.png");
            this.iconList.Images.SetKeyName(9, "png.png");
            this.iconList.Images.SetKeyName(10, "folder64.png");
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Font = new System.Drawing.Font("Times New Roman", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1011, 644);
            this.tabControl1.TabIndex = 7;
            // 
            // tabPage1
            // 
            this.tabPage1.BackgroundImage = global::process_explorer.Properties.Resources.image;
            this.tabPage1.Controls.Add(this.btnRefresh);
            this.tabPage1.Controls.Add(this.showDLLsBtn);
            this.tabPage1.Controls.Add(this.processDataGrid);
            this.tabPage1.Controls.Add(this.dllsList);
            this.tabPage1.Controls.Add(this.textBox1);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tabPage1.Size = new System.Drawing.Size(1003, 615);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Processes info";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // btnRefresh
            // 
            this.btnRefresh.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnRefresh.Font = new System.Drawing.Font("Times New Roman", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.btnRefresh.Location = new System.Drawing.Point(452, 11);
            this.btnRefresh.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnRefresh.Name = "btnRefresh";
            this.btnRefresh.Size = new System.Drawing.Size(98, 27);
            this.btnRefresh.TabIndex = 8;
            this.btnRefresh.Text = "Refresh";
            this.btnRefresh.UseVisualStyleBackColor = true;
            this.btnRefresh.Click += new System.EventHandler(this.btnRefresh_Click);
            // 
            // showDLLsBtn
            // 
            this.showDLLsBtn.Cursor = System.Windows.Forms.Cursors.Hand;
            this.showDLLsBtn.Font = new System.Drawing.Font("Times New Roman", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.showDLLsBtn.Location = new System.Drawing.Point(334, 11);
            this.showDLLsBtn.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.showDLLsBtn.Name = "showDLLsBtn";
            this.showDLLsBtn.Size = new System.Drawing.Size(99, 27);
            this.showDLLsBtn.TabIndex = 7;
            this.showDLLsBtn.Text = "Show DLLs";
            this.showDLLsBtn.UseVisualStyleBackColor = true;
            this.showDLLsBtn.Click += new System.EventHandler(this.showDLLsBtn_Click);
            // 
            // processDataGrid
            // 
            this.processDataGrid.AllowUserToAddRows = false;
            this.processDataGrid.AllowUserToDeleteRows = false;
            this.processDataGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.processDataGrid.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.AllCells;
            this.processDataGrid.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;
            this.processDataGrid.BackgroundColor = System.Drawing.Color.Honeydew;
            this.processDataGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.processDataGrid.Location = new System.Drawing.Point(16, 47);
            this.processDataGrid.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.processDataGrid.MultiSelect = false;
            this.processDataGrid.Name = "processDataGrid";
            this.processDataGrid.ReadOnly = true;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.processDataGrid.RowsDefaultCellStyle = dataGridViewCellStyle1;
            this.processDataGrid.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.processDataGrid.Size = new System.Drawing.Size(967, 431);
            this.processDataGrid.TabIndex = 6;
            // 
            // dllsList
            // 
            this.dllsList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dllsList.BackColor = System.Drawing.Color.Honeydew;
            this.dllsList.DisplayMember = "DLLs list";
            this.dllsList.ForeColor = System.Drawing.SystemColors.WindowText;
            this.dllsList.FormattingEnabled = true;
            this.dllsList.ItemHeight = 16;
            this.dllsList.Location = new System.Drawing.Point(16, 485);
            this.dllsList.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.dllsList.Name = "dllsList";
            this.dllsList.Size = new System.Drawing.Size(968, 84);
            this.dllsList.TabIndex = 0;
            // 
            // textBox1
            // 
            this.textBox1.Font = new System.Drawing.Font("Times New Roman", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.textBox1.Location = new System.Drawing.Point(18, 14);
            this.textBox1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBox1.Size = new System.Drawing.Size(279, 23);
            this.textBox1.TabIndex = 5;
            this.textBox1.Text = "<Filter by name>";
            this.textBox1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.textBox1_MouseClick);
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // tabPage2
            // 
            this.tabPage2.BackgroundImage = global::process_explorer.Properties.Resources.image;
            this.tabPage2.Controls.Add(this.tableLayoutPanel2);
            this.tabPage2.Controls.Add(this.fileTypeLabel);
            this.tabPage2.Controls.Add(this.goButton);
            this.tabPage2.Controls.Add(this.filePathTextBox);
            this.tabPage2.Controls.Add(this.backButton);
            this.tabPage2.Controls.Add(this.listView1);
            this.tabPage2.ForeColor = System.Drawing.Color.DarkSlateGray;
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tabPage2.Size = new System.Drawing.Size(1003, 615);
            this.tabPage2.TabIndex = 2;
            this.tabPage2.Text = "File system info";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.BackColor = System.Drawing.Color.Honeydew;
            this.tableLayoutPanel2.ColumnCount = 2;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 53.14417F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 46.85583F));
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel1, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.groupBox2, 1, 0);
            this.tableLayoutPanel2.Location = new System.Drawing.Point(14, 343);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(978, 249);
            this.tableLayoutPanel2.TabIndex = 21;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.BackColor = System.Drawing.Color.Honeydew;
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.labelName, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.labelOwner, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.labelAttributes, 0, 4);
            this.tableLayoutPanel1.Controls.Add(this.labelCreationTime, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.labelAccessTime, 0, 3);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 5);
            this.tableLayoutPanel1.ForeColor = System.Drawing.Color.DarkSlateGray;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(2, 2);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 6;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 13.02084F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 13.02083F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 13.02083F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 13.02083F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 13.02084F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 34.89584F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(513, 244);
            this.tableLayoutPanel1.TabIndex = 20;
            // 
            // labelName
            // 
            this.labelName.AutoSize = true;
            this.labelName.Location = new System.Drawing.Point(2, 0);
            this.labelName.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelName.Name = "labelName";
            this.labelName.Size = new System.Drawing.Size(43, 16);
            this.labelName.TabIndex = 9;
            this.labelName.Text = "Name:";
            // 
            // labelOwner
            // 
            this.labelOwner.AutoSize = true;
            this.labelOwner.Location = new System.Drawing.Point(2, 31);
            this.labelOwner.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelOwner.Name = "labelOwner";
            this.labelOwner.Size = new System.Drawing.Size(49, 16);
            this.labelOwner.TabIndex = 15;
            this.labelOwner.Text = "Owner:";
            // 
            // labelAttributes
            // 
            this.labelAttributes.AutoSize = true;
            this.labelAttributes.Location = new System.Drawing.Point(2, 124);
            this.labelAttributes.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelAttributes.Name = "labelAttributes";
            this.labelAttributes.Size = new System.Drawing.Size(65, 16);
            this.labelAttributes.TabIndex = 18;
            this.labelAttributes.Text = "Attributes:";
            // 
            // labelCreationTime
            // 
            this.labelCreationTime.AutoSize = true;
            this.labelCreationTime.Location = new System.Drawing.Point(2, 62);
            this.labelCreationTime.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelCreationTime.Name = "labelCreationTime";
            this.labelCreationTime.Size = new System.Drawing.Size(85, 16);
            this.labelCreationTime.TabIndex = 16;
            this.labelCreationTime.Text = "Creation time:";
            // 
            // labelAccessTime
            // 
            this.labelAccessTime.AutoSize = true;
            this.labelAccessTime.Location = new System.Drawing.Point(2, 93);
            this.labelAccessTime.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelAccessTime.Name = "labelAccessTime";
            this.labelAccessTime.Size = new System.Drawing.Size(104, 16);
            this.labelAccessTime.TabIndex = 17;
            this.labelAccessTime.Text = "Last access time:";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.aclList);
            this.panel1.Location = new System.Drawing.Point(2, 157);
            this.panel1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(507, 80);
            this.panel1.TabIndex = 19;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(0, 0);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(36, 16);
            this.label2.TabIndex = 13;
            this.label2.Text = "ACL:";
            // 
            // aclList
            // 
            this.aclList.FormattingEnabled = true;
            this.aclList.HorizontalScrollbar = true;
            this.aclList.ItemHeight = 16;
            this.aclList.Location = new System.Drawing.Point(39, 0);
            this.aclList.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.aclList.Name = "aclList";
            this.aclList.Size = new System.Drawing.Size(469, 68);
            this.aclList.TabIndex = 14;
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.AutoSize = true;
            this.groupBox2.BackColor = System.Drawing.Color.Honeydew;
            this.groupBox2.Controls.Add(this.btnOwnerChange);
            this.groupBox2.Controls.Add(this.addAccountBtn);
            this.groupBox2.Controls.Add(this.AccountTextBox);
            this.groupBox2.Controls.Add(this.groupBox3);
            this.groupBox2.Controls.Add(this.groupBox1);
            this.groupBox2.Controls.Add(this.btnShowUser);
            this.groupBox2.Controls.Add(this.btnShowGroup);
            this.groupBox2.Controls.Add(this.accountList);
            this.groupBox2.Controls.Add(this.btnApply);
            this.groupBox2.Controls.Add(this.securityRuleList);
            this.groupBox2.ForeColor = System.Drawing.Color.DarkSlateGray;
            this.groupBox2.Location = new System.Drawing.Point(521, 2);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox2.Size = new System.Drawing.Size(455, 245);
            this.groupBox2.TabIndex = 19;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Change object security";
            // 
            // btnOwnerChange
            // 
            this.btnOwnerChange.Location = new System.Drawing.Point(304, 168);
            this.btnOwnerChange.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnOwnerChange.Name = "btnOwnerChange";
            this.btnOwnerChange.Size = new System.Drawing.Size(137, 32);
            this.btnOwnerChange.TabIndex = 13;
            this.btnOwnerChange.Text = "Change owner";
            this.btnOwnerChange.UseVisualStyleBackColor = true;
            this.btnOwnerChange.Click += new System.EventHandler(this.btnOwnerChange_Click);
            // 
            // addAccountBtn
            // 
            this.addAccountBtn.Cursor = System.Windows.Forms.Cursors.Hand;
            this.addAccountBtn.ForeColor = System.Drawing.Color.DarkSlateGray;
            this.addAccountBtn.Location = new System.Drawing.Point(304, 15);
            this.addAccountBtn.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.addAccountBtn.Name = "addAccountBtn";
            this.addAccountBtn.Size = new System.Drawing.Size(138, 31);
            this.addAccountBtn.TabIndex = 12;
            this.addAccountBtn.Text = "Add account";
            this.addAccountBtn.UseVisualStyleBackColor = true;
            this.addAccountBtn.Click += new System.EventHandler(this.addAccountBtn_Click);
            // 
            // AccountTextBox
            // 
            this.AccountTextBox.Location = new System.Drawing.Point(11, 24);
            this.AccountTextBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.AccountTextBox.Name = "AccountTextBox";
            this.AccountTextBox.Size = new System.Drawing.Size(289, 23);
            this.AccountTextBox.TabIndex = 11;
            // 
            // groupBox3
            // 
            this.groupBox3.BackColor = System.Drawing.Color.Honeydew;
            this.groupBox3.Controls.Add(this.radioButtonRemove);
            this.groupBox3.Controls.Add(this.radioButtonAdd);
            this.groupBox3.ForeColor = System.Drawing.Color.DarkSlateGray;
            this.groupBox3.Location = new System.Drawing.Point(206, 163);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox3.Size = new System.Drawing.Size(93, 68);
            this.groupBox3.TabIndex = 10;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Action";
            // 
            // radioButtonRemove
            // 
            this.radioButtonRemove.AutoSize = true;
            this.radioButtonRemove.Location = new System.Drawing.Point(5, 40);
            this.radioButtonRemove.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.radioButtonRemove.Name = "radioButtonRemove";
            this.radioButtonRemove.Size = new System.Drawing.Size(71, 20);
            this.radioButtonRemove.TabIndex = 1;
            this.radioButtonRemove.TabStop = true;
            this.radioButtonRemove.Text = "Remove";
            this.radioButtonRemove.UseVisualStyleBackColor = true;
            // 
            // radioButtonAdd
            // 
            this.radioButtonAdd.AutoSize = true;
            this.radioButtonAdd.Location = new System.Drawing.Point(5, 18);
            this.radioButtonAdd.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.radioButtonAdd.Name = "radioButtonAdd";
            this.radioButtonAdd.Size = new System.Drawing.Size(48, 20);
            this.radioButtonAdd.TabIndex = 0;
            this.radioButtonAdd.TabStop = true;
            this.radioButtonAdd.Text = "Add";
            this.radioButtonAdd.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.BackColor = System.Drawing.Color.Honeydew;
            this.groupBox1.Controls.Add(this.radioButtonDeny);
            this.groupBox1.Controls.Add(this.radioButtonAllow);
            this.groupBox1.ForeColor = System.Drawing.Color.DarkSlateGray;
            this.groupBox1.Location = new System.Drawing.Point(109, 163);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox1.Size = new System.Drawing.Size(93, 68);
            this.groupBox1.TabIndex = 9;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Control type";
            // 
            // radioButtonDeny
            // 
            this.radioButtonDeny.AutoSize = true;
            this.radioButtonDeny.Location = new System.Drawing.Point(4, 40);
            this.radioButtonDeny.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.radioButtonDeny.Name = "radioButtonDeny";
            this.radioButtonDeny.Size = new System.Drawing.Size(55, 20);
            this.radioButtonDeny.TabIndex = 1;
            this.radioButtonDeny.TabStop = true;
            this.radioButtonDeny.Text = "Deny";
            this.radioButtonDeny.UseVisualStyleBackColor = true;
            // 
            // radioButtonAllow
            // 
            this.radioButtonAllow.AutoSize = true;
            this.radioButtonAllow.Location = new System.Drawing.Point(5, 18);
            this.radioButtonAllow.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.radioButtonAllow.Name = "radioButtonAllow";
            this.radioButtonAllow.Size = new System.Drawing.Size(58, 20);
            this.radioButtonAllow.TabIndex = 0;
            this.radioButtonAllow.TabStop = true;
            this.radioButtonAllow.Text = "Allow";
            this.radioButtonAllow.UseVisualStyleBackColor = true;
            // 
            // btnShowUser
            // 
            this.btnShowUser.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnShowUser.ForeColor = System.Drawing.Color.DarkSlateGray;
            this.btnShowUser.Location = new System.Drawing.Point(11, 203);
            this.btnShowUser.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnShowUser.Name = "btnShowUser";
            this.btnShowUser.Size = new System.Drawing.Size(93, 28);
            this.btnShowUser.TabIndex = 7;
            this.btnShowUser.Text = "Show users";
            this.btnShowUser.UseVisualStyleBackColor = true;
            this.btnShowUser.Click += new System.EventHandler(this.btnShowUser_Click);
            // 
            // btnShowGroup
            // 
            this.btnShowGroup.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnShowGroup.ForeColor = System.Drawing.Color.DarkSlateGray;
            this.btnShowGroup.Location = new System.Drawing.Point(11, 167);
            this.btnShowGroup.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnShowGroup.Name = "btnShowGroup";
            this.btnShowGroup.Size = new System.Drawing.Size(93, 32);
            this.btnShowGroup.TabIndex = 8;
            this.btnShowGroup.Text = "Show groups";
            this.btnShowGroup.UseVisualStyleBackColor = true;
            this.btnShowGroup.Click += new System.EventHandler(this.btnShowGroup_Click);
            // 
            // accountList
            // 
            this.accountList.FormattingEnabled = true;
            this.accountList.ItemHeight = 16;
            this.accountList.Location = new System.Drawing.Point(11, 52);
            this.accountList.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.accountList.Name = "accountList";
            this.accountList.Size = new System.Drawing.Size(289, 100);
            this.accountList.TabIndex = 5;
            // 
            // btnApply
            // 
            this.btnApply.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnApply.ForeColor = System.Drawing.Color.DarkSlateGray;
            this.btnApply.Location = new System.Drawing.Point(304, 203);
            this.btnApply.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.btnApply.Name = "btnApply";
            this.btnApply.Size = new System.Drawing.Size(138, 28);
            this.btnApply.TabIndex = 8;
            this.btnApply.Text = "Set access right";
            this.btnApply.UseVisualStyleBackColor = true;
            this.btnApply.Click += new System.EventHandler(this.btnApply_Click);
            // 
            // securityRuleList
            // 
            this.securityRuleList.FormattingEnabled = true;
            this.securityRuleList.ItemHeight = 16;
            this.securityRuleList.Items.AddRange(new object[] {
            "ChangePermissions",
            "Delete",
            "ExecuteFile",
            "FullControl",
            "Modify",
            "Read",
            "Write",
            "ReadAndExecute",
            "ListDirectory"});
            this.securityRuleList.Location = new System.Drawing.Point(304, 52);
            this.securityRuleList.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.securityRuleList.Name = "securityRuleList";
            this.securityRuleList.Size = new System.Drawing.Size(139, 100);
            this.securityRuleList.TabIndex = 6;
            // 
            // fileTypeLabel
            // 
            this.fileTypeLabel.AutoSize = true;
            this.fileTypeLabel.Location = new System.Drawing.Point(67, 393);
            this.fileTypeLabel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.fileTypeLabel.Name = "fileTypeLabel";
            this.fileTypeLabel.Size = new System.Drawing.Size(0, 16);
            this.fileTypeLabel.TabIndex = 12;
            // 
            // goButton
            // 
            this.goButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.goButton.Location = new System.Drawing.Point(936, 6);
            this.goButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.goButton.Name = "goButton";
            this.goButton.Size = new System.Drawing.Size(56, 22);
            this.goButton.TabIndex = 6;
            this.goButton.Text = "Go";
            this.goButton.UseVisualStyleBackColor = true;
            this.goButton.Click += new System.EventHandler(this.goButton_Click);
            // 
            // filePathTextBox
            // 
            this.filePathTextBox.Location = new System.Drawing.Point(75, 6);
            this.filePathTextBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.filePathTextBox.Name = "filePathTextBox";
            this.filePathTextBox.Size = new System.Drawing.Size(858, 23);
            this.filePathTextBox.TabIndex = 5;
            // 
            // backButton
            // 
            this.backButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.backButton.Location = new System.Drawing.Point(14, 6);
            this.backButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.backButton.Name = "backButton";
            this.backButton.Size = new System.Drawing.Size(56, 22);
            this.backButton.TabIndex = 4;
            this.backButton.Text = "Back";
            this.backButton.UseVisualStyleBackColor = true;
            this.backButton.Click += new System.EventHandler(this.backButton_Click);
            // 
            // listView1
            // 
            this.listView1.BackColor = System.Drawing.Color.Honeydew;
            this.listView1.ForeColor = System.Drawing.SystemColors.WindowText;
            this.listView1.HideSelection = false;
            this.listView1.LargeImageList = this.iconList;
            this.listView1.Location = new System.Drawing.Point(14, 40);
            this.listView1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(979, 310);
            this.listView1.SmallImageList = this.iconList;
            this.listView1.TabIndex = 3;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listView1_ItemSelectionChanged);
            this.listView1.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listView1_MouseDoubleClick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1011, 643);
            this.Controls.Add(this.tabControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Name = "Form1";
            this.Text = "Pingwin Hack";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.processDataGrid)).EndInit();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.ImageList iconList;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label labelName;
        private System.Windows.Forms.Label labelOwner;
        private System.Windows.Forms.Label labelAttributes;
        private System.Windows.Forms.Label labelCreationTime;
        private System.Windows.Forms.Label labelAccessTime;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ListBox aclList;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.RadioButton radioButtonRemove;
        private System.Windows.Forms.RadioButton radioButtonAdd;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton radioButtonDeny;
        private System.Windows.Forms.RadioButton radioButtonAllow;
        private System.Windows.Forms.Button btnShowUser;
        private System.Windows.Forms.Button btnShowGroup;
        private System.Windows.Forms.ListBox accountList;
        private System.Windows.Forms.Button btnApply;
        private System.Windows.Forms.ListBox securityRuleList;
        private System.Windows.Forms.Label fileTypeLabel;
        private System.Windows.Forms.Button goButton;
        private System.Windows.Forms.TextBox filePathTextBox;
        private System.Windows.Forms.Button backButton;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Button showDLLsBtn;
        private System.Windows.Forms.DataGridView processDataGrid;
        private System.Windows.Forms.ListBox dllsList;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.Button addAccountBtn;
        private System.Windows.Forms.TextBox AccountTextBox;
        private System.Windows.Forms.Button btnRefresh;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Button btnOwnerChange;
    }
}

