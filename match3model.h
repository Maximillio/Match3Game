#ifndef MATCH3MODEL_H
#define MATCH3MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QColor>
#include <QTimer>
#include <ball.h>
#include <random>
#include <chrono>
#include <algorithm>
#include <QDebug>


const int cellValue    = 10;
const int defaultDelay = 600;

enum Colors {None, Red, Green, Blue, Yellow, Orange};

class Match3Model : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool readyForInput MEMBER m_readyForInput READ readyForInput WRITE setReadyForInput NOTIFY readyForInputChanged);
    Q_PROPERTY(int score MEMBER m_score READ score WRITE setScore NOTIFY scoreChanged);
    Q_PROPERTY(bool readyForDisplay MEMBER readyForDisplay READ readyForDisplay WRITE setReadyForDisplay NOTIFY readyForDisplayChanged);
public:
    Match3Model();
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    bool readyForInput() const;
    void setReadyForInput(bool readyForInput);
    int score() const;
    void setScore(int score);
    bool readyForDisplay() const;
    void setReadyForDisplay(bool readyForDisplay);

signals:
    void readyForInputChanged();
    void scoreChanged();
    void readyForDisplayChanged();
public slots:
    Q_INVOKABLE bool swapCell(int _startIndex, int _endIndex);
    Q_INVOKABLE int colsNum() const;
    Q_INVOKABLE int rowsNum() const;
private:
    void initField();
    void resetInvisibleData();
    void swapGridCell(int _startIndex, int _endIndex);
    void checkCells();
    void checkRows();
    void checkColumns();
    void markCells(QList<int>& _markList);
    bool destroyCells();
    bool loadDataFromJSON();
    void addScores(int _scores);
    void enableDisplay();
    void disableDisplay();
private:
    QList<int>  m_destroyList;
    QList<Cell> m_field;
    int  m_colsNum;
    int  m_rowsNum;
    bool m_readyForInput;
    int  m_score           = 0;
    bool m_readyForDisplay = false;
    int m_startIndex       = 0;
    int m_endIndex         = 0;
    int delay              = 800;
};

#endif // MATCH3MODEL_H
