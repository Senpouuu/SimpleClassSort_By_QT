//
// Created by Senpouuu on 2023/11/23.
//

#include "course.h"
#include "mainwindow.h"
#include "QTableWidget"
extern QList<Room*> roomList;
std::pair<int, int> courseTable::posFinder(int day, int begin, int end) {
    std::pair<int, int> result;
    // 周一到周五
    for (int i = begin; i < end; i++)
    {
        if (table[i][day].getID() == -1)
        {
            result = std::make_pair(i, day);
            return result;
        }
    }
    return std::make_pair(-1,-1);
}

bool courseTable::arrangeCourse()
{
    clear();
    int sum = 0;
    std::sort(courseList.begin(),courseList.end(), compareMyClass);
    for (auto c:courseList)
    {
        // 数组下标从0到4
        auto flag = courseFiller(c,0,4);
        if (flag)
            sum++;
    }


    if (sum == courseList.length())
        return true;
    else
    {
        clear();
        sum = 0;
        std::sort(courseList.begin(),courseList.end(), compareMyClass);

        for (auto c:courseList)
        {
            auto flag = courseFiller(c,0,7);
            if (flag)
                sum++;
        }
    }
    if (sum == courseList.length())
        return true;
    else
        return false;
//    qDebug() << sum;
}





int courseTable::minDayGetter(int begin,int end)
{
    std::vector<int> arr(7,999);
    int sum = 0;
    for (int i = begin; i < end + 1 ;i++)
    {
        sum = 0;
        arr[i] = 0;
        for (int j = 0;j<ROWS;j++)
        {
            if(table[j][i].getID() != -1)
                sum++;
        }
        arr[i] = sum;
    }
    auto min = std::min_element(arr.begin(), arr.end());
    int minIndex = begin;
    if (min != arr.end())
        minIndex = std::distance(arr.begin(), min);

    return minIndex;
}


void courseTable::courseSetter(std::pair<int, int> pos, Course course) {
    int row = pos.first;
    int column = pos.second;
    for (int i = 0;i<course.getHouse();i++)
        table[row + i][column] = course;



}

int courseTable::posChecker(std::pair<int, int> idx, int courseHours) {
    int row = idx.first;
    // 0123
    if(row == -1)
        return 0;
    if (row <= 3)
    {
        int len = 4 - row;
        if(len >= courseHours)
            return 1;
        else
            return -1;
    }
    //4567
    else if (row <= 7)
    {
        int len = 8 - row;
        if(len >= courseHours)
            return 2;
        else
            return -2;
    }
    //89
    else if (row <= 9)
    {
        int len = 10 - row;
        if(len >= courseHours)
            return 3;
        else
            return -3;
    }
    return 0;
}

bool courseTable::courseFiller(Course course,int begin,int end) {

    int day = minDayGetter(begin,end);
    std::pair<int, int> idx = posFinder(day, 0, 4);
    int flag = posChecker(idx, course.getHouse());
    if (flag == 1)
    {
        courseSetter(idx, course);
        return true;
    }
    else if (flag == -1 || flag == 0)
    {
        idx = posFinder(day, 4, 8);
        flag = posChecker(idx, course.getHouse());
        if (flag == 2)
        {
            courseSetter(idx, course);
            return true;
        }
        else if (flag == -2 || flag == 0)
        {
            idx = posFinder(day, 8, 10);
            flag = posChecker(idx, course.getHouse());
            if (flag == 3)
            {
                courseSetter(idx, course);
                return true;
            }
            else if (flag == -3)
            {
                return false;
            }
        }
    }
    return false;
}

void courseTable::clear()
{
    Course temp[10][7];
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            table[i][j] = temp[i][j];
}

void courseTable::printSchedule(Course (*table)[7]) {
    int sum = 0;
    for (int i = 0; i < 7 - 1; ++i)
        for (int j = 0; j < 10 - 1; ++j)
            if (table[j][i].getID() != -1)
            {
                QString Name = "";
                if(Name!=table[i][j].getName())
                {
                    sum++;
                    Name = table[i][j].getName();
                }
                qDebug() << "索引 (列" << i << ", " << j << "行): "<< "课程ID: " << table[j][i].getID() << ", "<< "课程名称: " << table[j][i].getName() << ", "<< "学分: " << table[j][i].getCredits() << ", "<< "是否为专业基础课: " << (table[j][i].isCore() ? "是" : "否") << ", "<< "课时: " << table[j][i].getHouse() << ", "<< "老师: " << table[j][i].getTeacher();

            }
    qDebug() << "Total Classes:" <<sum;
}

void courseTable::setTableToWidget(Course (*table)[7],QTableWidget* widget)
{
    widget->clear();
    QStringList columnStrs = {"周一", "周二", "周三","周四","周五","周六","周日"};

    widget->setColumnCount(columnStrs.length());
    widget->setHorizontalHeaderLabels(columnStrs);

    QStringList rowStrs = {"上午", "上午", "上午","上午","下午","下午","下午","下午","晚上","晚上"};
    widget->setRowCount(rowStrs.length());
    widget->setVerticalHeaderLabels(rowStrs);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if(table[i][j].getID()!=-1)
            {
                auto item = new QTableWidgetItem(table[i][j].getName() + "\r\n" +table[i][j].getRoom());
                widget->setItem(i,j,item);
            }
        }
    }

}

void courseTable::setTableToWidget(QTableWidget *widget)
{
    widget->clear();
    QStringList columnStrs = {"周一", "周二", "周三","周四","周五","周六","周日"};
    widget->setColumnCount(columnStrs.length());
    widget->setHorizontalHeaderLabels(columnStrs);

    QStringList rowStrs = {"上午", "上午", "上午","上午","下午","下午","下午","下午","晚上","晚上"};
    widget->setRowCount(rowStrs.length());
    widget->setVerticalHeaderLabels(rowStrs);
    QString text;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 7; j++)
            if(table[i][j].getID()!=-1)
            {

                auto item = new QTableWidgetItem(table[i][j].getName() + "\r\n" +table[i][j].getRoom());
                widget->setItem(i,j,item);
            }
}



bool courseTable::arrangeRoomCourse(int begin,int end)
{
    int sum = 0;
    std::sort(courseList.begin(),courseList.end(), compareMyClass);
    for (auto c:courseList)
    {
        auto flag = courseFiller(c,begin,end);
        if (flag)
            sum++;
    }


    if (sum == courseList.length())
        return true;
    else
        return false;
}

bool courseTable::arrageCoursebyRoom(QString UserID)
{
    for (auto course: this->courseList)
    {
//        qDebug() <<"课程" + course.getName() << " 在当前MiniRoom:" + Room::getMinRoom(course.getID())->getID();
        auto pMinRoom = Room::getMinRoom(course.getID());
        pMinRoom->setCourseToCtable(UserID,course.getID(), this->table);
    }
    return true;
}

bool courseTable::arrageCoursebyRoom()
{


    for(auto r:roomList)
    {
        for(auto r:roomList)
            r->printStuTable();
        r->setRaisePlan("21312", this->courseList,table);
    }



    return true;
}

int *Room::findCoursePos(int courseID)
{
    int* pos = new int[3];
    pos[0] = -1;pos[1] = -1;pos[2] = -1;
    for (int i = 0; i < 7 - 1; ++i)
        for (int j = 0; j < 10 - 1; ++j)
            if(table[j][i].getID()==-1)
                continue;
            else if(table[j][i].getID() == courseID)
            {
                pos[0] = j;pos[1] = i;pos[2] = table[j][i].getHouse();
                return pos;
            }
    return pos;
}

/*根据课程号找到当前课程位置，将学生ID加入该课程*/
bool Room::setStudent(QString UserID,int courseID)
{
    auto pos = findCoursePos(courseID);
    if(pos[0] == -1)
        return false;
    if(stuTable[pos[0]][pos[1]].length() < 30)
    {
        stuTable[pos[0]][pos[1]].append(UserID);
        return true;
    }
    else
        return false;
}

bool Room::isFull(int courseID)
{
    auto pos = findCoursePos(courseID);
    if(stuTable[pos[0]][pos[1]].length() < 30)
        return true;
    else
        return false;
}

bool Room::setClassTable(Course (*table)[7],int courseID)
{
    auto pos = findCoursePos(courseID);
    if(pos[0] == -1)
        return false;
    auto course = Courses::getCourseByID(courseID);
    course.setRoom(this->roomID);
    for (int i =0;i < pos[2];i++)
        table[pos[0] + i][pos[1]] = course;

    return true;
}

/*根据学生的培养方案，将房间中的课程分配给table(课程表)*/
int Room::setRaisePlan(QString UserID,QList<Course> raisePlan,Course (*table)[7])
{
    bool flag = false;
    for (auto &r:raisePlan)
    {

        auto Course = Courses::getCourseByID(r.getID());
        flag = setClassTable(table,r.getID());
        setStudent(UserID,r.getID());
        if(!flag)
            continue;
    }
    if(flag)
        return 1;
    else
        return 0;
}



void Room::printStuTable()
{
    for (int i = 0; i < 7 - 1; ++i)
        for (int j = 0; j < 10 - 1; ++j)
            if (!stuTable[j][i].isEmpty())
                qDebug() <<"Room:"+ roomID +"  Index:("+ QString::number(i) + ',' + QString::number(j) + ")"<< stuTable[j][i];
}

QList<Room *> Room::roomsInit()
{
        std::sort(Courses::courses.begin(),Courses::courses.end(), courseTable::compareMyClass);
        int idx = 0;
        QList<Room*> Rooms;
        QList<Course> c1;
        c1.append(Courses::courses[0]);
        c1.append(Courses::courses[1]);
        c1.append(Courses::courses[10]);

        QList<Course> c2;
        c2.append(Courses::courses[2]);
        c2.append(Courses::courses[3]);
        c2.append(Courses::courses[11]);

        QList<Course> c3;
        c3.append(Courses::courses[4]);
        c3.append(Courses::courses[5]);
        c3.append(Courses::courses[12]);

        QList<Course> c4;
        c4.append(Courses::courses[6]);
        c4.append(Courses::courses[7]);
        c4.append(Courses::courses[13]);

        QList<Course> c5;
        c5.append(Courses::courses[8]);
        c5.append(Courses::courses[19]);
        c5.append(Courses::courses[9]);
        c5.append(Courses::courses[16]);

        QList<Course> c6;
        c6.append(Courses::courses[14]);
        c6.append(Courses::courses[15]);
        c6.append(Courses::courses[17]);
        c6.append(Courses::courses[18]);



        QList<Course> c7;
        c7.append(Courses::courses[7]);
        c7.append(Courses::courses[12]);
        c7.append(Courses::courses[13]);
        c7.append(Courses::courses[11]);

        QList<Course> c8;
        c8.append(Courses::courses[0]);
        c8.append(Courses::courses[9]);
        c8.append(Courses::courses[10]);

        QList<Course> c9;
        c9.append(Courses::courses[1]);
        c9.append(Courses::courses[4]);
        c9.append(Courses::courses[18]);

        QList<Course> c10;
        c10.append(Courses::courses[8]);
        c10.append(Courses::courses[5]);
        c10.append(Courses::courses[14]);

        QList<Course> c11;
        c11.append(Courses::courses[2]);
        c11.append(Courses::courses[3]);
        c11.append(Courses::courses[16]);


        QList<Course> c12;
        c12.append(Courses::courses[6]);
        c12.append(Courses::courses[15]);
        c12.append(Courses::courses[19]);
        c12.append(Courses::courses[17]);


        QList<Course> c13;
        c13.append(Courses::courses[5]);
        c13.append(Courses::courses[6]);
        c13.append(Courses::courses[14]);

        QList<Course> c14;
        c14.append(Courses::courses[0]);
        c14.append(Courses::courses[10]);
        c14.append(Courses::courses[11]);
        c14.append(Courses::courses[12]);

        QList<Course> c15;
        c15.append(Courses::courses[1]);
        c15.append(Courses::courses[8]);
        c15.append(Courses::courses[19]);

        QList<Course> c16;
        c16.append(Courses::courses[2]);
        c16.append(Courses::courses[3]);
        c16.append(Courses::courses[18]);

        QList<Course> c17;
        c5.append(Courses::courses[9]);
        c5.append(Courses::courses[13]);
        c5.append(Courses::courses[15]);
        c5.append(Courses::courses[16]);

        QList<Course> c18;
        c6.append(Courses::courses[4]);
        c6.append(Courses::courses[7]);
        c6.append(Courses::courses[17]);




        auto pRoom1 = new Room(c1,"C101",0,0);
        auto pRoom2 = new Room(c2,"C105",1,1);
        auto pRoom3 = new Room(c3,"C108",2,2);
        auto pRoom4 = new Room(c4,"C104",3,3);
        auto pRoom5 = new Room(c5,"C102",4,4);
        auto pRoom6 = new Room(c6,"C106",5,5);

        auto pRoom7 = new Room(c7,"C110",0,0);
        auto pRoom8 = new Room(c8,"C113",1,1);
        auto pRoom9 = new Room(c9,"C116",2,2);
        auto pRoom10 = new Room(c10,"C107",3,3);
        auto pRoom11 = new Room(c11,"C205",4,4);
        auto pRoom12 = new Room(c12,"C206",5,5);

        auto pRoom13 = new Room(c13,"C301",0,0);
        auto pRoom14 = new Room(c14,"C211",1,1);
        auto pRoom15 = new Room(c15,"C201",2,2);
        auto pRoom16 = new Room(c16,"C213",3,3);
        auto pRoom17 = new Room(c17,"C204",4,4);
        auto pRoom18 = new Room(c18,"C214",5,5);


        Rooms.append(pRoom1);
        Rooms.append(pRoom2);
        Rooms.append(pRoom3);
        Rooms.append(pRoom4);
        Rooms.append(pRoom5);
        Rooms.append(pRoom6);

        Rooms.append(pRoom7);
        Rooms.append(pRoom8);
        Rooms.append(pRoom9);
        Rooms.append(pRoom10);
        Rooms.append(pRoom11);
        Rooms.append(pRoom12);

        Rooms.append(pRoom13);
        Rooms.append(pRoom14);
        Rooms.append(pRoom15);
        Rooms.append(pRoom16);
        Rooms.append(pRoom17);
        Rooms.append(pRoom18);

        return Rooms;
}

int Room::getCurrentNum(int courseID)
{
    auto pos = findCoursePos(courseID);
    if (pos[0] == -1)
        return -1;
    return stuTable[pos[0]][pos[1]].length();
}

Room *Room::getMinRoom(int CourseID)
{
    QList<Room*> courseInRoom;
    for(auto r :roomList)
    {
        if (r->getCurrentNum(CourseID) == -1)
            continue;
        else
            courseInRoom.append(r);
    }

    Room * miniRoom = courseInRoom[0];
    for(auto r :courseInRoom)
        if(miniRoom->getCurrentNum(CourseID) > r->getCurrentNum(CourseID))
            miniRoom = r;

    return miniRoom;
}

void Room::setCourseToCtable(QString UserID,int ID,Course (*table)[7])
{
    auto Course = Courses::getCourseByID(ID);
    setClassTable(table,ID);
    setStudent(UserID,ID);
}


