#include "match3model.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

Match3Model::Match3Model()
{
    if (!loadDataFromJSON()) {
        m_rowsNum = 5;
        m_colsNum = 5;
    }
    initField();
    disableDisplay();
    checkCells();
}

int Match3Model::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_field.size();
}

QVariant Match3Model::data(const QModelIndex &index, int role) const {

    if (role == (Qt::UserRole + 1)) {
        return m_field[index.row()].m_type;
    }
    if (role == (Qt::UserRole + 2)) {
        switch (m_field[index.row()].m_type) {
            case Red:    {return "#ee3d3d"; break;}
            case Green:  {return "#5bf466"; break;}
            case Blue:   {return "#5278ff"; break;}
            case Yellow: {return "#f6fd87"; break;}
            case Orange: {return "#ffc53b"; break;}
        }
    }
    if (role == (Qt::UserRole + 3)) {
        return m_field[index.row()].m_checked;
    }

    return 0;
}

QHash<int, QByteArray> Match3Model::roleNames() const {
    QHash<int, QByteArray> role;
    role[Qt::UserRole + 1] = "typeValue";
    role[Qt::UserRole + 2] = "colorValue";
    role[Qt::UserRole + 3] = "isChecked";

    return role;
}

bool Match3Model::swapCell(int _startIndex, int _endIndex)
{
    bool ret = false;

    int difference = abs(_startIndex - _endIndex);
    if ((difference == 1) || (difference == m_colsNum)) {
        m_field.swap(_startIndex, _endIndex);
        swapGridCell(_startIndex, _endIndex);
        m_startIndex = _startIndex;
        m_endIndex   = _endIndex;
        ret = true;
    }
    if (ret) {

        checkCells();
    }
    return ret;
}

void Match3Model::swapGridCell(int _startIndex, int _endIndex)
{
    int action = _endIndex - _startIndex;
    if (action == -1) {
        beginMoveRows(QModelIndex(), _startIndex, _startIndex, QModelIndex(), _endIndex);
        endMoveRows();
    }
    if (action == 1) {
        beginMoveRows(QModelIndex(), _endIndex, _endIndex, QModelIndex(), _startIndex);
        endMoveRows();
    }
    if (action == m_colsNum) {
        beginMoveRows(QModelIndex(), _startIndex, _startIndex, QModelIndex(), _endIndex);
        endMoveRows();
        beginMoveRows(QModelIndex(), _endIndex, _endIndex, QModelIndex(), _startIndex);
        endMoveRows();
    }
    if (action == -m_colsNum) {
        beginMoveRows(QModelIndex(), _endIndex, _endIndex, QModelIndex(), _startIndex);
        endMoveRows();
        beginMoveRows(QModelIndex(), _startIndex, _startIndex, QModelIndex(), _endIndex);
        endMoveRows();
    }
}

void Match3Model::checkCells()
{
    static int destructionCycles = 0;

    checkRows();
    checkColumns();

    QTimer::singleShot(delay, [this](){
        setReadyForInput(false);
        if (destroyCells()) {
            QTimer::singleShot(delay, [this](){
                resetInvisibleData();
                QTimer::singleShot(delay, [this](){
                    checkCells();
                });
            });
            ++destructionCycles;
        }
        else {
            if (destructionCycles == 0) {
                m_field.swap(m_endIndex, m_startIndex);
                swapGridCell(m_endIndex, m_startIndex);
            }
            setReadyForInput(true);
            destructionCycles = 0;
            if (!m_readyForDisplay) {
                enableDisplay();
            }
        }
    });
}

void Match3Model::checkRows()
{
    QList<int> cellsToMark;

    for (int i = 0; i < m_rowsNum * m_colsNum; i += m_colsNum) {
        for (int j = 0; j < m_colsNum; ++j) {
            if (cellsToMark.empty()) {
                cellsToMark.push_back(i + j);
            }
            else {
                if (m_field[i + j].m_type == m_field[cellsToMark.first()].m_type) {
                    cellsToMark.push_back(i + j);
                }
                else {
                    if (cellsToMark.size() > 2) {
                        markCells(cellsToMark);
                        cellsToMark.clear();
                    }
                    else {
                        cellsToMark.clear();
                        cellsToMark.push_back(i + j);
                    }
                }
            }
        }
        if (cellsToMark.size() > 2) {
            markCells(cellsToMark);
        }
        cellsToMark.clear();
    }
}

void Match3Model::checkColumns()
{
    QList<int> cellsToMark;

    for (int i = 0; i < m_colsNum; ++i) {
        for (int j = i; j < m_rowsNum * m_colsNum; j += m_colsNum) {
            if (cellsToMark.empty()) {
                cellsToMark.push_back(j);
            }
            else {
                if (m_field[j].m_type == m_field[cellsToMark.first()].m_type) {
                    cellsToMark.push_back(j);
                }
                else {
                    if (cellsToMark.size() > 2) {
                        markCells(cellsToMark);
                        cellsToMark.clear();
                    }
                    else {
                        cellsToMark.clear();
                        cellsToMark.push_back(j);
                    }
                }
            }  
        }
        if (cellsToMark.size() > 2) {
            markCells(cellsToMark);
        }
        cellsToMark.clear();
    }
}

void Match3Model::markCells(QList<int> &_markList)
{
    for (int i = 0; i < _markList.size(); ++i) {
        if (!m_destroyList.contains(_markList[i])) {
            m_destroyList.push_back(_markList[i]);
        }
    }
}

bool Match3Model::destroyCells()
{
    bool ret = false;
    if (!m_destroyList.empty()) {
        //qDebug() << "Start destruction";
        foreach(int k, m_destroyList) {
            m_field[k].m_checked = true;
            //qDebug() << "Index: " << k << " with value: " << m_field[k].m_type;
        }
        //qDebug() << "End destruction";
        addScores(m_destroyList.size());
        std::sort(m_destroyList.begin(), m_destroyList.end(), [](int i, int j){return i > j;});
        dataChanged(index(0), index(m_rowsNum * m_colsNum));
        for (int j = 0; j < m_rowsNum; ++j) {
            for (int i = 0; i < m_destroyList.size(); ++i) {
                m_field.swap(m_destroyList[i], m_destroyList[i] + m_colsNum);
                swapGridCell(m_destroyList[i], m_destroyList[i] + m_colsNum);
                m_destroyList[i] += m_colsNum;
            }
        }
        ret = true;
    }
    else {
        ret = false;
    }
    m_destroyList.clear();

    return ret;
}

bool Match3Model::loadDataFromJSON()
{
    //Filestream initialisation:
    QString val;
    QFile   file;
    //File read:
    try {
        file.setFileName("config.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();
    }
    catch (std::exception ex) {
        qWarning("Error occured while reading JSON file");
        if (file.isOpen()) {
            file.close();
        }
        return false;
    }
    //Reading JSON file elements:
    bool ret = false;
    if (!val.isEmpty()) {
        QJsonDocument document   = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject globalObject = document.object();

        ret                      = globalObject["m_fieldRows"].isDouble();
        if (ret) {
            m_rowsNum            = globalObject["m_fieldRows"].toInt();
        }
        else {
            return ret;
        }
        ret                      = globalObject["m_fieldColumns"].isDouble();
        if (ret) {
            m_colsNum            = globalObject["m_fieldColumns"].toInt();
        }
        else {
            return ret;
        }
    }
    return ret;
}

void Match3Model::addScores(int _scores)
{
    m_score += _scores * cellValue;
    emit scoreChanged();
}

void Match3Model::enableDisplay()
{
    blockSignals(false);
    dataChanged(index(0), index(m_colsNum * m_rowsNum * 2 - 1));
    m_readyForDisplay = true;
    delay = defaultDelay;
    setScore(0);
    setReadyForInput(true);
    emit readyForDisplayChanged();
}

void Match3Model::disableDisplay()
{
    blockSignals(true);
    setReadyForInput(false);
}

bool Match3Model::readyForDisplay() const
{
    return m_readyForDisplay;
}

void Match3Model::setReadyForDisplay(bool readyForDisplay)
{
    m_readyForDisplay = readyForDisplay;
    emit readyForDisplayChanged();
}

int Match3Model::score() const
{
    return m_score;
}

void Match3Model::setScore(int score)
{
    m_score = score;
    emit scoreChanged();
}

bool Match3Model::readyForInput() const
{
    return m_readyForInput;
}

void Match3Model::setReadyForInput(bool readyForInput)
{
    m_readyForInput = readyForInput;
    emit readyForInputChanged();
}

int Match3Model::rowsNum() const
{
    return m_rowsNum;
}

void Match3Model::initField()
{
    srand(std::time(0));
    for (int i = 0; i < m_colsNum*m_rowsNum*2; ++i) {
        m_field.push_back(Cell());
        m_field[i].m_type = rand() % Orange + Red;
    }
}

void Match3Model::resetInvisibleData()
{
    srand(std::time(0));
    for (int i = m_colsNum*m_rowsNum; i < m_colsNum*m_rowsNum*2; ++i) {
        m_field[i].m_type = rand() % Orange + Red;
        m_field[i].m_checked = false;
    }
    dataChanged(index(m_colsNum*m_rowsNum), index(m_colsNum*m_rowsNum*2 - 1));
}

int Match3Model::colsNum() const
{
    return m_colsNum;
}
