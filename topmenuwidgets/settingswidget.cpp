#include "settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent)
{
    vLayout = new QVBoxLayout(this);
    vLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);

    setFixedSize(450, 220);
    setWindowTitle("Tools");

    setWindowIcon(QIcon(":/icon/settings.png"));

    // for clear memory after closed this window
    setAttribute(Qt::WA_ShowModal);
    setAttribute(Qt::WA_DeleteOnClose);

    createForm();
    //    gui_signal = QtCore.Signal()
    //    other_signal = QtCore.Signal()
}

void SettingsWidget::createForm()
{
//    if self.layout():
//        for i in range(self.layout().count()):
//            child = self.layout().takeAt(0)
//            child.widget().hide()
//            child.widget().destroy()
//            del (child)

    // Add clear config button
    clearConfigButton = new QPushButton(QIcon(":/icons/clear.png"),
                                        tr("Clear the configuration file"), this);
    clearConfigButton->setMaximumWidth(220);
    connect(clearConfigButton, &QPushButton::clicked, this, &SettingsWidget::clearConfig);

    vLayout->addWidget(clearConfigButton);

    mainTabWidget = new SettingsTabWidget(this);
    mainTabWidget->resize(mainTabWidget->sizeHint());
    vLayout->addWidget(mainTabWidget);

    // add ok, apply and cancel button
    buttonsWidget = new QWidget(this);
    buttonsLayout = new QHBoxLayout;
    buttonsLayout->setContentsMargins(0,0,0,0);
    buttonsLayout->setAlignment(Qt::AlignRight);

    okButton = new QPushButton(tr("Ok"), this);
    okButton->setFixedWidth(50);
    okButton->setShortcut(QKeySequence(Qt::Key_Return));
    connect(okButton, &QPushButton::clicked, this, &SettingsWidget::saveChanges);
    connect(okButton, &QPushButton::clicked, this, &SettingsWidget::close);
    buttonsLayout->addWidget(okButton);

    applyButton = new QPushButton(tr("Apply"), this);
    applyButton->setFixedWidth(100);
    connect(applyButton, &QPushButton::clicked, this, &SettingsWidget::saveChanges);
    buttonsLayout->addWidget(applyButton);

    cancelButton = new QPushButton(tr("Cancel"), this);
    cancelButton->setFixedWidth(100);
    cancelButton->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(cancelButton, &QPushButton::clicked, this, &SettingsWidget::close);
    buttonsLayout->addWidget(cancelButton);

    buttonsWidget->setLayout(buttonsLayout);

//    self.gui_signal.connect(tab.GuiWidget.save_changes(ClientObj))
//    self.other_signal.connect(tab.OtherWidget.save_changes(ClientObj))

    vLayout->addWidget(buttonsWidget);
}

void SettingsWidget::saveChanges()
{
}

void SettingsWidget::clearConfig()
{
}


//    def save_changes(self, ClientObj, tab):
//        def wrapper():
//            if type (tab.currentWidget()) == ToolGui:
//                self.gui_signal.emit()
//            else:
//                self.other_signal.emit()
//        return wrapper

//    def clear_config(self, parent, ClientObj):
//        def wrapper():
//            try:
//#            fc = open (ClientObj.user_config, 'w')
//#            fc.close()
//                os.remove(ClientObj.user_config)
//                ClientObj.create_user_config()
//                ClientObj.read_user_config(ClientObj.user_config)
//            except Exception, e:
//                show_msg (e, 'Clear Config Error')
//            finally:
//                self.create(parent, ClientObj)
//        return wrapper



//class ToolTabWidget(QtGui.QTabWidget):
//    def __init__(self, parent, ClientObj):
//        QtGui.QTabWidget.__init__(self, parent)
//        self.ClientObj = ClientObj

//        self.GuiWidget = ToolGui(self, ClientObj)

//        self.OtherWidget = ToolOther(self, ClientObj)

//        self.gui_icon = QtGui.QIcon.fromTheme("video-display")

//        self.other_icon = QtGui.QIcon.fromTheme("preferences-other")
//        if self.other_icon.isNull():
//            self.other_icon = QtGui.QIcon.fromTheme("preferences-desctop")

//        self.addTab(self.GuiWidget, self.gui_icon ,_('GUI Tools'))
//        self.addTab(self.OtherWidget, self.other_icon, _('Other Tools'))

//        # message about save
//        self.cur_tab_num = 0
//        self.changed_flag = False
//        self.currentChanged.connect(self.save_mess)

//        self.setAttribute(QtCore.Qt.WA_DeleteOnClose)

//    gui_signal = QtCore.Signal()
//    other_signal = QtCore.Signal()

//    def save_mess(self, tab_num):
//        # change tab with unsaved changes
//        tab_list = [_('GUI Tools'),_('Other Tools')]

//        if self.changed_flag:
//            text = _('There are unsaved changes in tab %s') \
//                                    %tab_list[self.cur_tab_num]
//            informative_text = _('<center>Apply them?</center>')

//            reply = show_question(self.parent(), text, informative_text,
//                                  title = _('Saving changes'))

//            if reply == QtGui.QMessageBox.Yes:
//                if self.cur_tab_num == 0:
//                    self.gui_signal.connect \
//                                  (self.GuiWidget.save_changes(self.ClientObj))
//                    self.gui_signal.emit()
//                elif self.cur_tab_num == 1:
//                    self.other_signal.connect \
//                                (self.OtherWidget.save_changes(self.ClientObj))
//                    self.other_signal.emit()

//            self.changed_flag = False
//            self.removeTab(self.cur_tab_num)

//            if self.cur_tab_num == 0:
//                self.GuiWidget = ToolGui(self, self.ClientObj)
//                self.insertTab(self.cur_tab_num, self.GuiWidget, \
//                                            self.gui_icon, _('GUI Tools'))
//            elif self.cur_tab_num == 1:
//                self.OtherWidget = ToolOther(self, self.ClientObj)
//                self.insertTab(self.cur_tab_num, self.OtherWidget, \
//                                        self.other_icon, _('Other Tools'))

//        self.cur_tab_num = tab_num
//        self.changed_flag = False

//# Gui tools in ToolTabWidget
//class ToolGui(QtGui.QWidget):
//    def __init__(self, parent, ClientObj):
//        QtGui.QWidget.__init__(self, parent)
//        self.user_config = ClientObj.user_config
//        self._parent = parent

//        self.grid = QtGui.QGridLayout(self)

//        self.grid.setContentsMargins(2,2,2,2)
//        self.grid.setSpacing(2)
//        self.grid.setColumnStretch(0,3)
//        self.grid.setColumnStretch(1,5)
//#        self.grid.setAlignment(QtCore.Qt.AlignLeft)

//        # add height image in grid
//        self.h_image_lbl = LabelWordWrap(_('Image height'), self)
//        self.h_image_lbl.setMaximumWidth(self.h_image_lbl.sizeHint().width())

//        self.h_image_lineedit = QtGui.QLineEdit(self)
//        self.h_image_lineedit.setToolTip(_('Set a fixed height image for '
//                                         'actions')+' '+_('0: hide images'))

//        self.h_image_lineedit.setText(str(ClientObj.height_image))

//        self.grid.addWidget(self.h_image_lbl, 0, 0)
//        self.grid.addWidget(self.h_image_lineedit, 0, 1)

//        # add expers view result in grid
//        self.expert_lbl = LabelWordWrap(_('Expert view'), self)
//        self.expert_checkbox = QtGui.QCheckBox(self)
//        self.expert_checkbox.setChecked(ClientObj.expert)
//        self.expert_lbl.setToolTip(_('View results in expert mode'))
//        self.expert_checkbox.setToolTip(_('View results in expert mode'))

//        self.grid.addWidget(self.expert_lbl, 1, 0)
//        self.grid.addWidget(self.expert_checkbox, 1, 1, 1, 2)

//        # add count item in result table in grid
//        self.count_row_lbl = LabelWordWrap(_('Rows in Table'),self)
//        self.count_row_lbl.setMaximumWidth \
//                                    (self.count_row_lbl.sizeHint().width())

//        self.count_row_lineedit = QtGui.QLineEdit(self)

//        self.count_row_lineedit.setText(str(ClientObj.count_row_res_table))

//        self.grid.addWidget(self.count_row_lbl, 2, 0)
//        self.grid.addWidget(self.count_row_lineedit, 2, 1)

//        # add spacer
//        self.grid.addItem(QtGui.QSpacerItem(0,0, QtGui.QSizePolicy.Expanding, \
//                          QtGui.QSizePolicy.Expanding ), 3, 0, 1, 2)

//        # connect all with change value slot
//        self.h_image_lineedit.textChanged.connect(self.changed_val)
//        self.expert_checkbox.clicked.connect(self.changed_val)
//        self.count_row_lineedit.textChanged.connect(self.changed_val)

//    def changed_val(self):
//        self._parent.changed_flag = True

//    def set_opacity_lbl(self, val):
//        self.opacity_lbl.setText(self.opacity_lbl_text + str(val))

//    def check_cfg (self, flag, config, part, param, value):
//        # if param not exists in config
//        if not flag:
//            part_flag = False
//            temp_cfg = []
//            for line in config:
//                temp_cfg.append(line)
//                #add new line in config
//                if line.startswith(part):
//                    temp_cfg.append('%s = %s\n' %(param, value))
//                    part_flag = True

//            config = temp_cfg
//            # if part not exists
//            if not part_flag:
//                config.append('\n')
//                config.append('%s\n' %part)
//                config.append('%s = %s\n' %(param, value))
//        return config

//    def save_changes(self, ClientObj):
//        def wrapper():
//            if not os.path.isfile (self.user_config):
//                f = open (self.user_config, 'w')
//                f.close()

//            fc = open (self.user_config, 'r')
//            config = fc.readlines()
//            fc.close()
//            new_config = []

//#            bg_color_flag = False
//            count_row_flag = False
//            expert_flag = False
//            count_row_flag = False
//            h_image_flag = False

//            for line in config:
//                if line.startswith('height_image '):
//                    h_image_flag = True
//                    try:
//                        height_image = int(self.h_image_lineedit.text())
//                    except ValueError:
//                        height_image = ClientObj.height_image
//                    if height_image < 0 or height_image > 512:
//                        height_image = ClientObj.height_image
//                    new_config.append('height_image = %d\n' %height_image)
//                elif line.startswith('expert '):
//                    expert_flag = True
//                    if self.expert_checkbox.isChecked():
//                        expert = 1
//                    else:
//                        expert = 0
//                    new_config.append('expert = %d\n' %expert)
//                elif line.startswith('count_row '):
//                    count_row_flag = True
//                    try:
//                        count_row = int(self.count_row_lineedit.text())
//                        if count_row < 2:
//                            count_row = 2
//                    except ValueError:
//                        count_row = ClientObj.count_row_res_table
//                    new_config.append('count_row = %d\n' %count_row)
//                else:
//                    new_config.append(line)

//            if self.expert_checkbox.isChecked():
//                expert = 1
//            else:
//                expert = 0
//            new_config = self.check_cfg (expert_flag, new_config, \
//                                '[gui]', 'expert', expert)

//            try:
//                count_row = int(self.count_row_lineedit.text())
//                if count_row < 2:
//                    count_row = 2
//            except ValueError:
//                count_row = ClientObj.count_row_res_table
//            new_config = self.check_cfg (count_row_flag, new_config, \
//                                '[gui]', 'count_row', count_row)

//            try:
//                height_image = int(self.h_image_lineedit.text())
//            except ValueError:
//                height_image = ClientObj.height_image
//            if height_image < 0 or height_image > 512:
//                height_image = ClientObj.height_image
//            new_config = self.check_cfg (h_image_flag, new_config, \
//                                '[gui]', 'height_image', height_image)

//            fnc = open(self.user_config, 'w')
//            for line in new_config:
//                fnc.write(line)
//            fnc.close()

//            # read config for changed parameters
//            ClientObj.create_user_config()
//            ClientObj.read_user_config(ClientObj.user_config)

//            # reset unsaved changes flag
//            self._parent.changed_flag = False
//        return wrapper

//# Other tools in ToolTabWidget
//class ToolOther(QtGui.QWidget):
//    def __init__(self, parent, ClientObj):
//        QtGui.QWidget.__init__(self, parent)
//        self.user_config = ClientObj.user_config
//        self._parent = parent

//        self.grid = QtGui.QGridLayout(self)

//        self.grid.setContentsMargins(2,2,2,2)
//        self.grid.setSpacing(2)
//        self.grid.setColumnStretch(0,3)
//        self.grid.setColumnStretch(1,5)

//        # lang settings
//        self.lang_lbl = LabelWordWrap(_('Select Language'), self)
//        self.lang_lbl.setMaximumWidth(self.lang_lbl.sizeHint().width())

//        self.lang_ComboBox = QComboBox(self)
//        lang_dict = {'en': _('English'),'ru': _('Russian'),'fr':_('French')}

//        for lang in lang_dict:
//            self.lang_ComboBox.addItem(lang_dict[lang])
//            self.lang_ComboBox.setItemData(self.lang_ComboBox.count()-1,lang)
//            if ClientObj.lang == lang:
//                self.lang_ComboBox.setCurrentIndex \
//                                               (self.lang_ComboBox.count()-1)

//        #add lang settings in grid
//        self.grid.addWidget(self.lang_lbl,0,0)
//        self.grid.addWidget(self.lang_ComboBox,0,1)

//        # add open file in grid
//        self.cert_path_lbl = LabelWordWrap(_('Path to Certificates'), self)
//        self.cert_path_lbl.setMaximumWidth \
//                                      (self.cert_path_lbl.sizeHint().width())

//        self.fd_cert = FileOpenWgt(self, 'dir', _('Certificate Directory'), \
//                                   '~/.calculate')

//        self.fd_cert.setToolTip(_('Empty to default path'))
//        self.fd_cert.setText(ClientObj.path_to_cert)

//        self.grid.addWidget(self.cert_path_lbl, 1, 0)
//        self.grid.addWidget(self.fd_cert, 1, 1)

//#        # add timeout in grid
//#        self.timeout_lbl = LabelWordWrap(_('Timeout'), self)
//#        self.timeout_lbl.setMaximumWidth(self.timeout_lbl.sizeHint().width())
//#
//#        self.timeout_lineedit = QtGui.QLineEdit(self)
//#
//#        self.timeout_lineedit.setText(str(ClientObj.timeout))
//#
//#        self.grid.addWidget(self.timeout_lbl, 2, 0)
//#        self.grid.addWidget(self.timeout_lineedit, 2, 1)

//        # add spacer
//        self.grid.addItem(QtGui.QSpacerItem(0,0, QtGui.QSizePolicy.Expanding, \
//                                QtGui.QSizePolicy.Expanding ), 5, 0, 1, 2)

//        # connect all with change value slot
//        self.lang_ComboBox.currentIndexChanged.connect(self.changed_val)
//        self.fd_cert.textChanged.connect(self.changed_val)
//#        self.timeout_lineedit.textChanged.connect(self.changed_val)

//        self.setAttribute(QtCore.Qt.WA_DeleteOnClose)

//    def changed_val(self):
//        self._parent.changed_flag = True

//    def check_cfg (self, flag, config, part, param, value):
//        # if param not exists in config
//        if not flag:
//            part_flag = False
//            temp_cfg = []
//            for line in config:
//                temp_cfg.append(line)
//                #add new line in config
//                if line.startswith(part):
//                    temp_cfg.append('%s = %s\n' %(param, value))
//                    part_flag = True

//            config = temp_cfg
//            # if part not exists
//            if not part_flag:
//                config.append('\n')
//                config.append('%s\n' %part)
//                config.append('%s = %s\n' %(param, value))
//        return config

//    def save_changes(self, ClientObj):
//        def wrapper():
//            if not os.path.isfile (self.user_config):
//                f = open (self.user_config, 'w')
//                f.close()

//            fc = open (self.user_config, 'r')
//            config = fc.readlines()
//            fc.close()
//            new_config = []

//            lang_flag = False
//            cert_flag = False
//#            timeout_flag = False
//            for line in config:
//                if line.startswith('lang '):
//                    lang_flag = True
//                    new_config.append('lang = %s\n' \
//                                %self.lang_ComboBox.itemData \
//                                (self.lang_ComboBox.currentIndex()))
//                elif line.startswith('path_to_cert '):
//                    cert_flag = True
//                    if os.path.isdir(self.fd_cert.text()):
//                                    new_config.append('path_to_cert = %s\n' \
//                                    %self.fd_cert.text())
//                    elif not self.fd_cert.text():
//                                    new_config.append('path_to_cert = no\n')
//                    else:
//                        new_config.append(line)
//#                elif line.startswith('timeout '):
//#                    timeout_flag = True
//#                    try:
//#                        timeout = int(self.timeout_lineedit.text())
//#                    except ValueError:
//#                        timeout = ClientObj.timeout
//#                    new_config.append('timeout = %d\n' %timeout)
//                else:
//                    new_config.append(line)

//            new_config = self.check_cfg (lang_flag, new_config, \
//                              '[other]', 'lang', self.lang_ComboBox.itemData \
//                              (self.lang_ComboBox.currentIndex()))

//            if not self.fd_cert.text().lower():
//                new_config = self.check_cfg (cert_flag, new_config, \
//                                '[other]', 'path_to_cert', 'no')
//            elif os.path.isdir(self.fd_cert.text()):
//                new_config = self.check_cfg (cert_flag, new_config, \
//                              '[other]', 'path_to_cert', self.fd_cert.text())

//#            try:
//#                timeout = int(self.timeout_lineedit.text())
//#            except ValueError:
//#                timeout = ClientObj.timeout
//#            new_config = self.check_cfg (timeout_flag, new_config, \
//#                                '[other]', 'timeout', timeout)

//            fnc = open(self.user_config, 'w')
//            for line in new_config:
//                fnc.write(line)
//            fnc.close()

//            # read config for changed parameters
//            ClientObj.create_user_config()
//            ClientObj.read_user_config(ClientObj.user_config)

//            # reset unsaved changes flag
//            self._parent.changed_flag = False
//            if ClientObj.client:
//                from session_function import client_post_cert
//                ClientObj.lang = self.lang_ComboBox.itemData \
//                                           (self.lang_ComboBox.currentIndex())
//                if ClientObj.client:
//                    try:
//                        client_post_cert(ClientObj.client, ClientObj.lang)
//                    except:
//                        return
//                    ClientObj.methods_list = client_list_methods \
//                                            (ClientObj.sid, ClientObj.client)
//                    from DisplayMethod import DisplayMethod
//                    if type (ClientObj.MainWidget.MainFrameWgt) \
//                                                            == DisplayMethod:
//                        ClientObj.MainWidget.display_methods()
//        return wrapper
