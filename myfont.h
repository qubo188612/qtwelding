#ifndef MYFONT_H
#define MYFONT_H


//主页背景色
#define FONT_MAIN_BACKGROUND_INFO "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(251,102,102, 200), stop:1 rgba(20,196,188, 210));"

//主页按钮大字体
#define FONT_MAIN_BUTTON_BIG_INFO  "QPushButton{font: 22pt '黑体 Light';color: rgb(0,0,0);background-color: rgb(20,196,188);"     \
                                   "border: none;border-radius:16px;}"      \
                                   "QPushButton:hover{background-color: rgb(22,218,208);}"      \
                                   "QPushButton:pressed{background-color: rgb(17,171,164);}"        \
                                   "QPushButton:disabled{color: rgb(90,90,90);background-color: rgb(180,180,180);}"

//主页按钮小字体
#define FONT_MAIN_BUTTON_SMALL_INFO  "QPushButton{font: 16pt '黑体 Light';color: rgb(0,0,0);background-color: rgb(20,196,188);"     \
                                     "border: none;border-radius:16px;}"      \
                                     "QPushButton:hover{background-color: rgb(22,218,208);}"      \
                                     "QPushButton:pressed{background-color: rgb(17,171,164);}"        \
                                     "QPushButton:disabled{color: rgb(90,90,90);background-color: rgb(180,180,180);}"

//主页编辑框
#define FONT_MAIN_EDIT_INFO     "color: rgb(31,31,31);"     \
                                "background-color: rgb(255, 255, 255);"     \
                                "border:2px solid rgb(20,196,188);border-radius:5px;"

//主页组
#define FONT_MAIN_GROUPBOX_INFO     "background-color:rgba(192,192,192,255);"      \
                                    "border-radius:15px;"       \
                                    "color:rgb(0,0,0);"

//主页单选框
/*
#define FONT_MAIN_CHECKBOX_INFO     "QCheckBox:indicator{ background-color: rgb(240,240,240); "     \
                                    "border:2px solid rgb(20,196,188);}"
*/


#endif // MYFONT_H
