#include <stdio.h>
#include <string.h>

struct Widget {
    const char* name;
    const char* operation;
};


Widget widgets[20] = {
    { "QLineEdit",    "setText(QString::number(%s))" },
    { "QProgressBar", "setValue(%s)" },
    { "QScrollBar",   "setValue(%s)" },
    { "QLCDNumber",   "display(QString::number(%s))" },
    { "QTextEdit",    "print(QPrinter(%s)" },
    { "QDial",        "setValue(%s)" },
    { "QCheckBox",    "setChecked(%s != 0)" },
    { 0, 0 },
};


int main(int pn, char** pv) {


    char sourceData[50] = "";
    if(pn == 2) strcpy(sourceData, pv[1]);
    printf("// data source %s\n", sourceData);
    

    char inputline[1024];
    while(fgets(inputline, 1024, stdin)) { // for all lines

        // class name
        char* classNm = strstr(inputline,"<class>");
        if(classNm) {
            classNm += 7; // skipt <class>
            char* endpos = strchr(classNm, '<');
            *endpos = 0;
            printf("\nvoid %s::printAllContent() {\n", classNm);
            continue;
        }

        // Search for widgets
        for(int i = 0; widgets[i].name ; i++) { // for all widgets names in each line
            if(strstr(inputline, widgets[i].name))  { // widget name found?
                char* name =  strstr(inputline,"name=");
                if(name) {
                    name += 6; // skip name="
                    char* endpos = strchr(name, '\"');
                    *endpos = 0;
                    char formatedText1[100];
                    char formatedText2[100];
                    sprintf(formatedText1, "%s%s", sourceData, name);
                    sprintf(formatedText2, widgets[i].operation, formatedText1);
                    printf("    ui->%s->%s;\n", name, formatedText2);
                } // if name is defined
                break;
            } // if widget name was found
        } // test search for all widgnet named
    } // for all lines
    printf("}\n");
    return 0;
}
