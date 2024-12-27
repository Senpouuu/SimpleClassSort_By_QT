//
// Created by Senpouuu on 2023/11/23.
//

#ifndef QT_TEMPLATE_COURSE_H
#define QT_TEMPLATE_COURSE_H
#include "iostream"
#include "QString"
#include "QList"
#include "QTableWidget"
class Course{
public:
    // 构造函数，用于初始化课程对象
    Course(int id, QString name, int credits, bool isCore,int hour,QString teacher)
            : courseID(id), courseName(name), courseCredits(credits), isCoreCourse(isCore) ,courseHours(hour) ,courseTeacher(teacher){}

    Course()
    {
        this->courseID = -1;
        this->courseHours = -1;
        this->isCoreCourse = -1;
        this->courseCredits = -1;
        this->courseName = "";
        this->courseTeacher = "";
        this->roomID = "";
    };

    // 成员函数，用于获取课程的ID
    int getID() const
    {
        return courseID;
    }

    // 成员函数，用于获取课程的名称
    QString getName() const
    {
        return courseName;
    }

    // 成员函数，用于获取课程的学分
    int getCredits() const
    {
        return courseCredits;
    }

    // 成员函数，用于检查是否为专业基础课
    bool isCore() const
    {
        return isCoreCourse;
    }

    QString getTeacher()
    {
        return courseTeacher;
    }

    int getHouse()
    {
        return courseHours;
    }
    QString getRoom()
    {
        return roomID;
    }
    void setRoom(QString roomID)
    {
        this->roomID = roomID;
    }
private:
    int courseID;           // 课程ID
    QString courseName;  // 课程名称
    int courseCredits;       // 学分
    bool isCoreCourse;       // 是否为专业基础课
    int courseHours;      // 课时
    QString roomID;
    QString courseTeacher;     // 老师
};


class Courses
{
public:

    static QList<Course> getCourses()
    {
        QList<Course> courses;
        return courses;
    }
    static void appendCourse(Course c)
    {
        courses.append(c);
    }
    static Course getCourseByName(QString Name)
    {
        for (auto c : courses)
            if (c.getName() == Name)
                return c;
        Course c;
        return c;
    }
    static Course getCourseByID(int ID)
    {
        for (auto c : courses)
            if (c.getID() == ID)
                return c;
        Course c;
        return c;
    }

    static inline QList<Course> courses = getCourses();


};

class courseTable
{
public:
    courseTable(QList<Course> list)
    {
        this->courseList = list;
    }

    courseTable();
    bool arrangeCourse();
    bool arrageCoursebyRoom(QString UserID);
    bool arrageCoursebyRoom();
    bool arrangeRoomCourse(int begin,int end);
    static void printSchedule(Course (*table)[7]);
    void setTableToWidget(QTableWidget* widget);
    static void setTableToWidget(Course (*table)[7],QTableWidget* widget);
    static bool compareMyClass(Course& a, Course& b){return a.getHouse() > b.getHouse();}

    Course table[10][7];            // 要显示的课程表
private:
    int ROWS = 10,COLS = 7;
    QList<Course> courseList;       // 记录学生的培养方案


    // 从day开始，找begin到end的空位
    std::pair<int, int> posFinder(int day,int begin,int end);
    int posChecker(std::pair<int,int> idx,int courseHours);
    void courseSetter(std::pair<int,int> pos,Course course);
    int minDayGetter(int begin,int end);
    bool courseFiller(Course course,int begin,int end);
    void clear();

};

    /* 每个专业每个学生的培养方案都是不一样的，但是相同专业学生的培养方案基本上是相同的，也就是说他们上的课基本上是相同的，同一个专业课
     * 可以有多个老师上课，也就是说时间可以相同，但是房间可以不同。不同的专业课房间可以相同，但是时间必须不同，基本上，一个学生能选择的专业课是有限的
     * 选修课也是有限的，如果一天只能存放10小时的课程，专业课时间为4小时，那么房间课程的安排基本上为 首先是442 然后是332 或是3312 最后22222
     * 然后将这些组合分配给每一个房间中，在为每个学生分配房间时，根据房间中分配的课程将学生分配到房间中去，这个过程中可以选择根据当前课程人数分配学生
     * 或是一些其他的策略，目前到此为止，不再继续优化。 2023-12-9 15:45:47*/
class Room:public courseTable
{
private:
    QString roomID;
    QList<QString> stuTable[10][7];         //房间课程学生数
    std::vector<Course*> courseList;

public:
    Room(QList<Course> list,QString ID,int begin,int end):courseTable(list),roomID(ID){arrangeRoomCourse(begin,end);}
    bool isFull(int courseID);
    bool setStudent(QString UserID,int courseID);
    int setRaisePlan(QString UserID,QList<Course> raisePlan,Course (*table)[7]);
    void setCourseToCtable(QString UserID,int ID,Course (*table)[7]);
    bool setClassTable(Course (*table)[7],int courseID);
    int* findCoursePos(int courseID);
    int getCurrentNum(int courseID);
    void printStuTable();
    static QList<Room*> roomsInit();
    static Room* getMinRoom(int CourseID);
    QString getID(){return roomID;}


};


#endif //QT_TEMPLATE_COURSE_H
