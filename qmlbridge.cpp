#include <QtWidgets/QMessageBox>

#include <iostream>
#include <cassert>
#include <unistd.h>

#include "qmlbridge.h"
#include "core/keypad.h"

QMLBridge::QMLBridge(QObject *p) : QObject(p) {
}

QMLBridge::~QMLBridge() {
}

static constexpr const int ROWS = 8, COLS = 8;

void QMLBridge::keypadStateChanged(int keymap_id, bool state) {
    int col = keymap_id % COLS, row = keymap_id / COLS;
    assert(row < ROWS);

    keypad_key_event(row, col, state);
}

static QObject *buttons[ROWS][COLS];

void QMLBridge::registerNButton(int keymap_id, QVariant button) {
    int col = keymap_id % COLS, row = keymap_id / COLS;
    assert(row < ROWS);

    if(buttons[row][col])
        qWarning() << "Warning: Button " << keymap_id << " already registered as " << buttons[row][col] << "!";
    else
        buttons[row][col] = button.value<QObject*>();
}


void notifyKeypadStateChanged(int row, int col, bool state) {
    assert(row < ROWS);
    assert(col < COLS);

    if(!buttons[row][col])
    {
        qWarning() << "Warning: Button " << row*11+col << " not present in keypad!";
        return;
    }

    QQmlProperty::write(buttons[row][col], "state", state);
}

QObject *qmlBridgeFactory(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new QMLBridge();
}