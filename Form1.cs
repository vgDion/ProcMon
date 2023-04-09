using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.DirectoryServices;
using System.IO;
using System.Linq;
using System.Security.AccessControl;
using System.Security.Principal;
using System.Windows.Forms;
using System.Windows;
using System.Diagnostics;
using ProcessPrivileges;

namespace explorer
{

    public partial class Form1 : Form
    {
        DataTable dt;
        bool sameDir = true;
        List<processItem> processes;
        List<FileInfo> files = new List<FileInfo>();
        private string filePath = "C:\\";
        private bool isFile = false;
        private string currentlySelectedItemName = "";
        public Form1()
        {
            InitializeComponent();
        }

        private void textBox1_MouseClick(object sender, MouseEventArgs e)
        {
            textBox1.Text = "";
        }
        public void LoadJson()
        {

            using (StreamReader r = new StreamReader("ProcessHacker.json"))
            {
                string json = r.ReadToEnd();
                processes = JsonConvert.DeserializeObject<List<processItem>>(json);
            }

        }

        public void tableUpdate()
        {
            for (int i = 0; i < processes.Count; i++)
            {
                DataRow dr = dt.NewRow();
                dr[0] = processes[i].name;
                dr[1] = processes[i].PID;
                dr[2] = processes[i].parentPID;
                dr[3] = processes[i].parentName;
                dr[4] = processes[i].exePath;
                dr[5] = processes[i].description;
                dr[6] = processes[i].SID;
                dr[7] = processes[i].ownerName;
                dr[8] = processes[i].processType;
                dr[9] = processes[i].compilationMode;
                dr[10] = processes[i].depPolicy;
                dr[11] = processes[i].aslrPolicy;
                dr[12] = processes[i].integrityLevel;
                dt.Rows.Add(dr);
            }
            processDataGrid.DataSource = dt;
        }
        public class processItem
        {
            public string name;
            public int PID;
            public int parentPID;
            public string parentName;
            public string exePath;
            public string description;
            public string SID;
            public string ownerName;
            public string processType;
            public string compilationMode;
            public string depPolicy;
            public string aslrPolicy;
            public string integrityLevel;
            public List<string> dlls = new List<string>();


        }
        public DataGridViewColumn CreateNewColumn(string header, string name)
        {
            DataGridViewColumn column = new DataGridViewColumn();
            column.HeaderText = header; //текст в шапке
            column.ReadOnly = true; //значение в этой колонке нельзя править
            column.Name = name; //текстовое имя колонки, его можно использовать вместо обращений по индексу
            column.Frozen = false; //флаг, что данная колонка всегда отображается на своем месте
            column.CellTemplate = new DataGridViewTextBoxCell(); //тип нашей колонки
            return column;
        }
        public DataColumn CreateDataColumn(string name)
        {
            DataColumn column = new DataColumn();
            column.ColumnName = name;
            return column;
        }

        private void Form1_Load(object sender, EventArgs e)
        {


            Process proc = Process.GetProcessById(Process.GetCurrentProcess().Id);
            try
            {
                Process.EnterDebugMode();

            }
            catch (Win32Exception)
            {
                // The local process isn't running in administrator mode
                MessageBox.Show("Please run as administrator to use all options.", "Warning");
                System.Windows.Forms.Application.Exit();
            }
            Process process = Process.GetCurrentProcess();
            new PrivilegeEnabler(process, Privilege.TakeOwnership);
            new PrivilegeEnabler(process, Privilege.Debug);
            new PrivilegeEnabler(process, Privilege.Restore);
            filePathTextBox.Text = filePath;
            loadFilesAndDirectories();
            try
            {
                LoadJson();
            }
            catch (System.IO.FileNotFoundException)
            {
                MessageBox.Show("Something went wrong...", "Error");
            }
            List<DataColumn> columnList = new List<DataColumn>();
            List<DataGridViewColumn> gridColumnList = new List<DataGridViewColumn>();
            dt = new DataTable();
            columnList.Add(CreateDataColumn("Name"));
            columnList.Add(CreateDataColumn("PID"));
            columnList.Add(CreateDataColumn("Parent PID"));
            columnList.Add(CreateDataColumn("Parent Name"));
            columnList.Add(CreateDataColumn("Exe path"));
            columnList.Add(CreateDataColumn("Description"));
            columnList.Add(CreateDataColumn("SID"));
            columnList.Add(CreateDataColumn("Owner name"));
            columnList.Add(CreateDataColumn("Process type"));
            columnList.Add(CreateDataColumn("Compilatin mode"));
            columnList.Add(CreateDataColumn("DEP policy"));
            columnList.Add(CreateDataColumn("ASLR policy"));
            columnList.Add(CreateDataColumn("Integrity level"));
            for (int i = 0; i < columnList.Count(); i++)
                dt.Columns.Add(columnList[i]);
            try
            {
                tableUpdate();
            }
            catch
            {

            }

        }

        public void loadFilesAndDirectories()
        {
            DirectoryInfo fileList;
            if (isFile)
            {

            }
            else
            {
                sameDir = false;
                fileList = new DirectoryInfo(filePathTextBox.Text);
                try
                {
                    FileInfo[] files = fileList.GetFiles(); // GET ALL THE FILES
                    DirectoryInfo[] dirs = fileList.GetDirectories(); // GET ALL THE DIRS
                    string fileExtension = "";
                    listView1.Items.Clear();
                    for (int i = 0; i < files.Length; i++)
                    {
                        fileExtension = files[i].Extension.ToUpper();
                        switch (fileExtension)
                        {
                            case ".MP3":
                            case ".MP2":
                                listView1.Items.Add(files[i].Name, 5);
                                break;
                            case ".EXE":
                            case ".COM":
                                listView1.Items.Add(files[i].Name, 7);
                                break;

                            case ".MP4":
                            case ".AVI":
                            case ".MKV":
                                listView1.Items.Add(files[i].Name, 6);
                                break;
                            case ".PDF":
                                listView1.Items.Add(files[i].Name, 4);
                                break;
                            case ".DOC":
                            case ".DOCX":
                                listView1.Items.Add(files[i].Name, 3);
                                break;
                            case ".PNG":
                            case ".JPG":
                            case ".JPEG":
                                listView1.Items.Add(files[i].Name, 9);
                                break;
                            case ".SYS":
                                break;
                            case ".TMP":
                                break;
                            default:
                                listView1.Items.Add(files[i].Name, 8);
                                break;
                        }
                    }
                    for (int i = 0; i < dirs.Length; i++)
                        listView1.Items.Add(dirs[i].Name, 10);
                }
                catch (System.IO.DirectoryNotFoundException)
                {
                    MessageBox.Show("Incorrect path!", "Error");
                }
                catch (System.UnauthorizedAccessException)
                {
                    sameDir = true;
                }
            }
        }

        private void goButton_Click(object sender, EventArgs e)
        {
            //isFile = false;
            loadFilesAndDirectories();
        }

        private void listView1_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            currentlySelectedItemName = e.Item.Text;
            if (sameDir)
                while (filePathTextBox.Text[filePathTextBox.Text.Length - 1] != '\\')
                    filePathTextBox.Text = filePathTextBox.Text.Remove(filePathTextBox.Text.Length - 1, 1);
            sameDir = true;
            filePathTextBox.Text = Path.Combine(filePathTextBox.Text, currentlySelectedItemName);
            string path = filePathTextBox.Text;
            aclList.Items.Clear();
            try
            {
                FileAttributes fileAttr = File.GetAttributes(path);
                if ((fileAttr & FileAttributes.Directory) == FileAttributes.Directory)
                {
                    isFile = false;
                    DirectoryInfo dirDetails = new DirectoryInfo(path);
                    labelName.Text = "Name: " + dirDetails.Name;
                    labelAccessTime.Text = "Last access time: " + dirDetails.LastAccessTime.ToString();
                    labelCreationTime.Text = "Creation time: " + dirDetails.CreationTime.ToString();
                    labelAttributes.Text = "Attributes: " + dirDetails.Attributes.ToString();
                    DirectorySecurity dSecurity = null;
                    try { dSecurity = Directory.GetAccessControl(path); }
                    catch (System.InvalidOperationException ex)
                    {
                        // MessageBox.Show("System directory.", "Error");
                    }
                    catch (System.UnauthorizedAccessException ex)
                    {
                        //MessageBox.Show("System directory.", "Error");
                    }
                    string s;
                    if (dSecurity != null)
                    {
                        labelOwner.Text = "Owner: " + dSecurity.GetOwner(typeof(NTAccount)).ToString() + " (" + dSecurity.GetOwner(typeof(SecurityIdentifier)).ToString() + ")";
                        foreach (FileSystemAccessRule fileRule in
                                   dSecurity.GetAccessRules(true, true, typeof(System.Security.Principal.NTAccount)))
                        {
                            bool skip = false;
                            string fsr = "";
                            Int32 r = (Int32)fileRule.FileSystemRights;
                            switch (r)
                            {
                                case -536805376:
                                    skip = true;
                                    fsr = "Unknown access right";
                                    break;
                                case -1610612736:
                                    fsr = "Unknown access right";
                                    skip = true;
                                    break;
                                case 268435456:
                                    fsr = "Unknown access right";
                                    skip = true;
                                    break;
                                default:
                                    fsr = fileRule.FileSystemRights.ToString();
                                    break;
                            }
                            s = fileRule.AccessControlType == AccessControlType.Allow ? "Allowed" : "Denied";
                            aclList.Items.Add(s + " access " + fsr + " for " + fileRule.IdentityReference.ToString() + " \n");
                        }
                    }
                }
                else
                {
                    isFile = true;
                    FileInfo fileDetails = new FileInfo(filePathTextBox.Text);
                    labelName.Text = "Name: " + fileDetails.Name;
                    labelAccessTime.Text = "Last access time: " + fileDetails.LastAccessTime.ToString();
                    labelCreationTime.Text = "Creation time: " + fileDetails.CreationTime.ToString();
                    labelAttributes.Text = "Attributes: " + fileDetails.Attributes.ToString();
                    FileSecurity fSecurity = null;
                    try { fSecurity = File.GetAccessControl(path); }
                    catch (System.InvalidOperationException ex)
                    {

                    }
                    catch (System.UnauthorizedAccessException)
                    {
                        MessageBox.Show("Not ehough rights.", "Error");
                    }
                    string s;
                    if (fSecurity != null)
                    {
                        labelOwner.Text = "Owner: " + fSecurity.GetOwner(typeof(NTAccount)).ToString() + " (" + fSecurity.GetOwner(typeof(SecurityIdentifier)).ToString() + ")";
                        foreach (FileSystemAccessRule fileRule in
                                   fSecurity.GetAccessRules(true, true, typeof(System.Security.Principal.NTAccount)))
                        {
                            s = fileRule.AccessControlType == AccessControlType.Allow ? "Allowed" : "Denied";
                            aclList.Items.Add(s + " access " + fileRule.FileSystemRights.ToString() + " for " + fileRule.IdentityReference.ToString() + " \n");
                        }

                    }
                }
            }
            catch (System.IO.FileNotFoundException)
            {
                MessageBox.Show("File system object not found.", "Error");
                while (filePathTextBox.Text[filePathTextBox.Text.Length - 1] != '\\')
                    filePathTextBox.Text = filePathTextBox.Text.Remove(filePathTextBox.Text.Length - 1, 1);
                loadFilesAndDirectories();
            }
        }

        private void listView1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            //filePathTextBox.Text = Path.Combine(filePathTextBox.Text, currentlySelectedItemName);
            loadFilesAndDirectories();

        }

        private void backButton_Click(object sender, EventArgs e)
        {

            isFile = false;
            if (filePathTextBox.Text[filePathTextBox.Text.Length - 1] == '\\' && filePathTextBox.Text[filePathTextBox.Text.Length - 2] != ':')
            {
                filePathTextBox.Text = filePathTextBox.Text.Remove(filePathTextBox.Text.Length - 1, 1);
                while (filePathTextBox.Text[filePathTextBox.Text.Length - 1] != '\\')
                    filePathTextBox.Text = filePathTextBox.Text.Remove(filePathTextBox.Text.Length - 1, 1);
            }
            else if (filePathTextBox.Text[filePathTextBox.Text.Length - 1] != '\\')
            {
                while (filePathTextBox.Text[filePathTextBox.Text.Length - 1] != '\\')
                    filePathTextBox.Text = filePathTextBox.Text.Remove(filePathTextBox.Text.Length - 1, 1);
            }
            loadFilesAndDirectories();

        }

        public void GetComputerUsers(string account)
        {
            List<string> users = new List<string>();
            List<string> users_domain = new List<string>();
            var path =
                string.Format("WinNT://{0},computer", Environment.MachineName);
            Console.WriteLine(path);
            using (var computerEntry = new DirectoryEntry(path))
                foreach (DirectoryEntry childEntry in computerEntry.Children)
                    if (childEntry.SchemaClassName == account)
                        users.Add(childEntry.Name);
            foreach (string user in users)
            {
                NTAccount a = new NTAccount(user);
                SecurityIdentifier s = (SecurityIdentifier)a.Translate(typeof(SecurityIdentifier));
                NTAccount b = (NTAccount)s.Translate(typeof(NTAccount));
                users_domain.Add(b.ToString());
            }
            accountList.DataSource = users_domain;
        }


        private void showDLLsBtn_Click(object sender, EventArgs e)
        {
            string proc_name = processDataGrid.Rows[processDataGrid.CurrentRow.Index].Cells[0].Value.ToString();
            for (int i = 0; i < processes.Count(); i++)
            {
                if (processes[i].name == proc_name)
                {
                    dllsList.DataSource = processes[i].dlls;
                    break;
                }
            }

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

            (processDataGrid.DataSource as DataTable).DefaultView.RowFilter = $"Name LIKE '%{textBox1.Text}%'";

        }
        private void btnShowUser_Click(object sender, EventArgs e)
        {
            GetComputerUsers("User");

        }

        private void btnShowGroup_Click(object sender, EventArgs e)
        {
            GetComputerUsers("Group");
        }
        public static void RemoveFileSecurity(string fileName, string account,
           FileSystemRights rights, AccessControlType controlType, bool objectType)
        {
            if (objectType)
            {
                // Get a FileSecurity object that represents the
                // current security settings.
                FileSecurity fSecurity = File.GetAccessControl(fileName);
                // Remove the FileSystemAccessRule from the security settings.
                try
                {
                    fSecurity.RemoveAccessRule(new FileSystemAccessRule(account,
                     rights, controlType));
                }
                catch (ArgumentException e)
                {
                    MessageBox.Show("File don't has such security right.", "Error");
                }

                // Set the new access settings.
                File.SetAccessControl(fileName, fSecurity);
            }
            else
            {
                DirectorySecurity dSecurity = Directory.GetAccessControl(fileName);
                // Remove the FileSystemAccessRule from the security settings.
                try
                {
                    dSecurity.RemoveAccessRule(new FileSystemAccessRule(account,
                     rights, controlType));
                }
                catch (ArgumentException e)
                {
                    MessageBox.Show("File don't has such security right.", "Error");
                }
                // Set the new access settings.
                Directory.SetAccessControl(fileName, dSecurity);
            }
        }

        public static void AddFileSecurity(string fileName, string account,
            FileSystemRights rights, AccessControlType controlType, bool objectType)
        {
            // Get a FileSecurity object that represents the
            // current security settings.
            if (objectType)
            {
                FileSecurity fSecurity = File.GetAccessControl(fileName);
                // Add the FileSystemAccessRule to the security settings.
                try
                {
                    fSecurity.AddAccessRule(new FileSystemAccessRule(account,
                    rights, controlType));
                }
                catch (ArgumentException e)
                {
                    MessageBox.Show("File has security rigth with opposite control type.", "Error");
                }
                // Set the new access settings.
                File.SetAccessControl(fileName, fSecurity);
            }
            else
            {
                DirectorySecurity dSecurity = Directory.GetAccessControl(fileName);
                try
                {
                    dSecurity.AddAccessRule(new FileSystemAccessRule(account,
                    rights, controlType));
                }
                catch (ArgumentException e)
                {
                    MessageBox.Show("File has security rigth with opposite control type.", "Error");
                }
                // Set the new access settings.
                Directory.SetAccessControl(fileName, dSecurity);
            }
        }

        public FileSystemRights getFileRight(string rule)
        {
            FileSystemRights rights = new FileSystemRights();
            switch (rule)
            {
                case "ChangePermissions":
                    rights = FileSystemRights.ChangePermissions;
                    break;
                case "Delete":
                    rights = FileSystemRights.Delete;
                    break;
                case "ExecuteFile":
                    rights = FileSystemRights.ExecuteFile;
                    break;
                case "FullControl":
                    rights = FileSystemRights.FullControl;
                    break;
                case "Modify":
                    rights = FileSystemRights.Modify;
                    break;
                case "Read":
                    rights = FileSystemRights.Read;
                    break;
                case "Write":
                    rights = FileSystemRights.Write;
                    break;
                case "ReadAndExecute":
                    rights = FileSystemRights.ReadAndExecute;
                    break;
                case "ListDirectory":
                    rights = FileSystemRights.ListDirectory;
                    break;


            }
            return rights;
        }

        private void btnApply_Click(object sender, EventArgs e)
        {
            string s;
            string user = "";
            string rule = "";
            bool action = false;
            FileSecurity fSecurity;
            DirectorySecurity dSecurity;
            if ((radioButtonAdd.Checked || radioButtonRemove.Checked) && (radioButtonAllow.Checked || radioButtonDeny.Checked) && (accountList.SelectedItem != null) && (securityRuleList.SelectedItem != null))
            {

                user = accountList.SelectedItem.ToString();
                rule = securityRuleList.SelectedItem.ToString();
                AccessControlType controlType = new AccessControlType();
                if (radioButtonAllow.Checked)
                    controlType = AccessControlType.Allow;
                else if (radioButtonDeny.Checked)
                    controlType = AccessControlType.Deny;
                if (radioButtonAdd.Checked)
                    action = true;
                else if (radioButtonRemove.Checked)
                    action = false;
                FileSystemRights rights = getFileRight(rule);
                fSecurity = null;
                dSecurity = null;
                if (isFile)
                {
                    try
                    { fSecurity = File.GetAccessControl(filePathTextBox.Text); }
                    catch (UnauthorizedAccessException ex)
                    {
                        MessageBox.Show(ex.Message.ToString() + " Запустите приложение от имени администратора.", "Error");
                    }
                    if (action)
                        AddFileSecurity(filePathTextBox.Text, user, rights, controlType, isFile);
                    else
                        RemoveFileSecurity(filePathTextBox.Text, user, rights, controlType, isFile);
                    try { fSecurity = File.GetAccessControl(filePathTextBox.Text); }

                    catch (UnauthorizedAccessException ex)
                    {
                        MessageBox.Show(ex.Message.ToString() + " Запустите приложение от имени администратора.", "Error");
                    }
                    if (fSecurity != null)
                    {
                        aclList.Items.Clear();
                        foreach (FileSystemAccessRule fileRule in fSecurity.GetAccessRules(true, true, typeof(System.Security.Principal.NTAccount)))
                        {
                            s = fileRule.AccessControlType == AccessControlType.Allow ? "Allowed" : "Denied";
                            aclList.Items.Add(s + " access " + fileRule.FileSystemRights.ToString() + " for " + fileRule.IdentityReference.ToString() + " \n");

                        }
                    }

                }
                else
                {
                    try
                    { dSecurity = Directory.GetAccessControl(filePathTextBox.Text); }
                    catch (UnauthorizedAccessException ex)
                    {
                        MessageBox.Show(ex.Message.ToString() + " Запустите приложение от имени администратора.", "Error");
                    }
                    if (action)
                        AddFileSecurity(filePathTextBox.Text, user, rights, controlType, isFile);
                    else
                        RemoveFileSecurity(filePathTextBox.Text, user, rights, controlType, isFile);
                    try { dSecurity = Directory.GetAccessControl(filePathTextBox.Text); }

                    catch (UnauthorizedAccessException ex)
                    {
                        MessageBox.Show(ex.Message.ToString() + " Запустите приложение от имени администратора.", "Error");
                    }
                    if (dSecurity != null)
                    {
                        aclList.Items.Clear();
                        foreach (FileSystemAccessRule fileRule in dSecurity.GetAccessRules(true, true, typeof(System.Security.Principal.NTAccount)))
                        {
                            s = fileRule.AccessControlType == AccessControlType.Allow ? "Allowed" : "Denied";
                            aclList.Items.Add(s + " access " + fileRule.FileSystemRights.ToString() + " for " + fileRule.IdentityReference.ToString() + " \n");

                        }
                    }
                }
            }
            else
                MessageBox.Show("Select all options!", "Warning");
        }

        private void addAccountBtn_Click(object sender, EventArgs e)
        {
            string user = AccountTextBox.Text;
            if (user != "")
            {
                List<string> addedAccounts = new List<string>();
                NTAccount a = new NTAccount(user);
                try
                {
                    SecurityIdentifier s = (SecurityIdentifier)a.Translate(typeof(SecurityIdentifier));
                    NTAccount b = (NTAccount)s.Translate(typeof(NTAccount));
                    addedAccounts.Add(b.ToString());
                    accountList.DataSource = addedAccounts;
                }
                catch (System.Security.Principal.IdentityNotMappedException ex)
                {
                    MessageBox.Show("Invalid user or group name.", "Error");
                }
            }

        }

        private void btnRefresh_Click(object sender, EventArgs e)
        {
            Process.Start("ProcessHacker.exe");
            processes.Clear();
            LoadJson();
            tableUpdate();
        }


        private void btnOwnerChange_Click(object sender, EventArgs e)
        {
            string user = "";

            FileAttributes fileAttr = File.GetAttributes(filePathTextBox.Text);
            if ((fileAttr & FileAttributes.Directory) == FileAttributes.Directory)
            {
                DirectorySecurity dSecurity = null;
                try { dSecurity = Directory.GetAccessControl(filePathTextBox.Text); }
                catch (System.InvalidOperationException ex)
                {
                    // MessageBox.Show("System directory.", "Error");
                }
                catch (System.UnauthorizedAccessException ex)
                {
                    //MessageBox.Show("System directory.", "Error");
                }
                string s;
                if (dSecurity != null)
                {
                    if (accountList.SelectedItem != null)
                    {
                        user = accountList.SelectedItem.ToString();
                        NTAccount a = new NTAccount(user);
                        dSecurity.SetOwner(a);
                        try
                        {

                            Directory.SetAccessControl(filePathTextBox.Text, dSecurity);

                        }

                        catch (InvalidOperationException ex)
                        {
                        }
                        // Privilege is enabled within the using block.

                        dSecurity = Directory.GetAccessControl(filePathTextBox.Text);
                        labelOwner.Text = "Owner: " + dSecurity.GetOwner(typeof(NTAccount)).ToString() + " (" + dSecurity.GetOwner(typeof(SecurityIdentifier)).ToString() + ")";
                    }
                    else
                        MessageBox.Show("Select an account.", "Error");
                }

            }
            else
            {
                FileSecurity fSecurity = null;
                try { fSecurity = File.GetAccessControl(filePathTextBox.Text); }
                catch (System.InvalidOperationException ex)
                {
                    // MessageBox.Show("System directory.", "Error");
                }
                catch (System.UnauthorizedAccessException ex)
                {
                    //MessageBox.Show("System directory.", "Error");
                }
                string s;
                if (fSecurity != null)
                {
                    if (accountList.SelectedItem != null)
                    {
                        user = accountList.SelectedItem.ToString();
                        NTAccount a = new NTAccount(user);

                        fSecurity.SetOwner(a);
                        try
                        {

                            File.SetAccessControl(filePathTextBox.Text, fSecurity);

                        }

                        catch (InvalidOperationException ex)
                        {
                        }
                        // Privilege is enabled within the using block.

                        fSecurity = File.GetAccessControl(filePathTextBox.Text);
                        labelOwner.Text = "Owner: " + fSecurity.GetOwner(typeof(NTAccount)).ToString() + " (" + fSecurity.GetOwner(typeof(SecurityIdentifier)).ToString() + ")";
                    }
                    else
                        MessageBox.Show("Select an account.", "Error");
                }
            }

        }

    }
}


