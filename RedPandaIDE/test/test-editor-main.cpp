#include <QTest>
#include <QGuiApplication>
#include "test_editor_symbol_completion.h"
#include "test_editor_session_save.h"

int main(int argc, char *argv[]) {
    int status = 0;
    QTest::setMainSourcePath(__FILE__, QT_TESTCASE_BUILDDIR); // Optional: for source path resolution

    QApplication app(argc,argv);
    //CharPos Test
    {
        TestEditorSymbolCompletion tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    {
        TestEditorSessionSave tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
    return status;
}
