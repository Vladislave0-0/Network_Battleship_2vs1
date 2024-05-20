#include "field.hpp"

Field::Field() :
    width_(FIELD_WIDTH_DEFAULT),
    height_(FIELD_HEIGHT_DEFAULT),
    area_(FIELD_WIDTH_DEFAULT*FIELD_HEIGHT_DEFAULT)
{
    clear();
}

Field::~Field()
{

}

Field::Field(QString field, QString fieldState) :
    width_(FIELD_WIDTH_DEFAULT),
    height_(FIELD_HEIGHT_DEFAULT),
    area_(FIELD_WIDTH_DEFAULT*FIELD_HEIGHT_DEFAULT)
{
    setField(field);
    setFieldState(fieldState);
}

Cell Field::getCell(int x, int y)
{
    qDebug() << "Field::getCell";

    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        qDebug() << "HERE: " + QString::number(width_*y+x);
        return field_[width_*y+x];
//        return CELL_SHIP;
    }

    qDebug() << "Wrong cell indexes";
    return Cell::CELL_EMPTY;
}

void Field::setCell(int x, int y, Cell cell)
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        field_[width_*y+x] = cell;
        return;
    }

    qDebug() << "ERROR: no such cell (" << x << "," << y << ")";
}

QString Field::getFieldStr()
{
    QString result = "";

    for(QVector<Cell>::iterator cell_it = field_.begin(); cell_it != field_.end(); ++cell_it)
    {
        result += QString::number(*cell_it);
    }

    return result;
}

void Field::setField(QString field)
{
    field_.clear();

    for(QString::iterator cell_it = field.begin(); cell_it != field.end(); ++cell_it)
    {
        if (cell_it->digitValue() < (int)CELL_EMPTY || cell_it->digitValue() > (int)CELL_SHIP)
        {
            qDebug() << "setField(str): wrong string!";
            field_.clear();
            return;
        }

        field_.push_back((Cell)cell_it->digitValue());
    }
}

void Field::initFieldDraw()
{
    fieldDraw_.clear();

    for(QVector<Cell>::iterator cell_it = field_.begin(); cell_it != field_.end(); ++cell_it)
    {
        if(*cell_it == Cell::CELL_EMPTY)
        {
            fieldDraw_.push_back(CellDraw::CELL_EMPTY);
        }
        else if(*cell_it == Cell::CELL_SHIP)
        {
            fieldDraw_.push_back(CellDraw::CELL_LIVE);
        }
    }
}

void Field::setFieldState(QString field)
{
    fieldState_.clear();

    for(QString::iterator cell_it = field.begin(); cell_it != field.end(); ++cell_it)
    {
        if (cell_it->digitValue() < (int)CL_ST_EMPTY || cell_it->digitValue() > (int)CL_ST_UNDEFINED)
        {
            qDebug() << "setField(str): wrong string!";
            fieldState_.clear();
            return;
        }

        fieldState_.push_back((CellState)cell_it->digitValue());
    }
}

//void Field::setField(QVector<Cell> field)
//{
//    if (field.size() != area_)
//        return;

//    field_ = field;
//}

void Field::clear()
{
    field_.fill(Cell::CELL_EMPTY, area_);
}

bool Field::isCellEmpty(int x, int y)
{
    qDebug() << "Field::isCellEmpty";
    return getCell(x, y) == Cell::CELL_EMPTY;
}

bool Field::isCorrect() const  // check if ship placement is correct
{
    // TODO: check for correct or not

    return true;
}

void Field::generate()
{
    qDebug() << "\"generate\" clicked: Generating new field";

    // TODO: add generated fields and atabase

    QString field_example = "1111011100"\
                            "0000000000"\
                            "1110110110"\
                            "0000000000"\
                            "1101010101"\
                            "0000000000"\
                            "0000000000"\
                            "0000000000"\
                            "0000000000"\
                            "0000000000";   // example for testing

    setField(field_example);

    qDebug() << "Generated field (state): " + getFieldStr();
}

bool Field::isKilled(int x, int y)
{
    QVector<CellState> fieldStateWithBorders((width_+2)*(height_+2));
    QVector<CellDraw> fieldDrawWithBorders((width_+2)*(height_+2));
    fieldStateWithBorders.fill(CL_ST_EMPTY);
    fieldDrawWithBorders.fill(CELL_EMPTY);

    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            fieldStateWithBorders[(width_+2)*(i+1)+(j+1)] = fieldState_[width_*i+j];
        }
    }

    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            fieldDrawWithBorders[(width_+2)*(i+1)+(j+1)] = fieldDraw_[width_*i+j];
        }
    }

    int damagedCell = (width_+2)*(x+1)+(y+1);

    switch(fieldStateWithBorders[damagedCell])
    {
        case(CL_ST_CENTER):
        {
            return true;
        }

        case(CL_ST_TOP):
        {
            for(int curCell = damagedCell + 12; fieldStateWithBorders[curCell] != CL_ST_EMPTY; curCell += 12)
            {
                if(fieldDrawWithBorders[curCell] != CELL_KILLED && fieldDrawWithBorders[curCell] != CELL_DAMAGED)
                    return false;
            }
            return true;
        }

        case(CL_ST_LEFT):
        {
            for(int curCell = damagedCell + 1; fieldStateWithBorders[curCell] != CL_ST_EMPTY; ++curCell)
            {
                if(fieldDrawWithBorders[curCell] != CELL_KILLED && fieldDrawWithBorders[curCell] != CELL_DAMAGED)
                    return false;
            }
            return true;
        }

        case(CL_ST_BOTTOM):
        {
            for(int curCell = damagedCell - 12; fieldStateWithBorders[curCell] != CL_ST_EMPTY; curCell -= 12)
            {
                if(fieldDrawWithBorders[curCell] != CELL_KILLED && fieldDrawWithBorders[curCell] != CELL_DAMAGED)
                    return false;
            }
            return true;
        }

        case(CL_ST_VMIDDLE):
        {
            int curCell = damagedCell;
            for(; fieldStateWithBorders[curCell] != CL_ST_EMPTY; curCell -= 12) {} // сначала находим верхнюю границу, потом идем вправо

            for(; fieldStateWithBorders[curCell] != CL_ST_EMPTY; curCell += 12)
            {
                if(fieldDrawWithBorders[curCell] != CELL_KILLED && fieldDrawWithBorders[curCell] != CELL_DAMAGED)
                    return false;
            }
            return true;
        }

        case(CL_ST_HMIDDLE):
        {
            int curCell = damagedCell;
            for(; fieldStateWithBorders[curCell] != CL_ST_EMPTY; --curCell) {} // сначала находим левую границу, потом идем вправо

            for(; fieldStateWithBorders[curCell] != CL_ST_EMPTY; ++curCell)
            {
                if(fieldDrawWithBorders[curCell] != CELL_KILLED && fieldDrawWithBorders[curCell] != CELL_DAMAGED)
                    return false;
            }

            return true;
        }

        case(CL_ST_RIGHT):
        {
            for(int curCell = damagedCell - 1; fieldStateWithBorders[curCell] != CL_ST_EMPTY; --curCell)
            {
                if(fieldDrawWithBorders[curCell] != CELL_KILLED && fieldDrawWithBorders[curCell] != CELL_DAMAGED)
                    return false;
            }
            return true;
        }

        default:
        {
            qDebug() << "From function isKilled: unknown cell state!";
            return false;
        }
    }

    return false;
}


void Field::setCellState(int x, int y, CellState state)
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        fieldState_[width_*y+x] = state;
        return;
    }
}

void Field::setDrawState(int x, int y, CellDraw state)
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        fieldDraw_[width_*y+x] = state;
        return;
    }
}
