#include "weather.h"
#include "ui_weather.h"


//week
QString week[7] = {"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};

weather_t today_weather = {"","","","","","","",""};
weather_t feature1_weather;
weather_t feature2_weather;
weather_t feature3_weather;
weather_t feature4_weather;

lunar_t lunar_day;

Weather::Weather(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Weather)
{
    ui->setupUi(this);
    this->ui->pushButton->setFocusPolicy(Qt::NoFocus);

    //设置背景颜色
    QPalette palette;
    palette.setColor(QPalette::Background,QColor(97,111,125));
    this->setPalette(palette);

    //1.新建网络请求 管理器
    manager  = new QNetworkAccessManager(this);
    manager2 = new QNetworkAccessManager(this);
    manager3 = new QNetworkAccessManager(this);

    //关联管理器的  finished 信号
    connect(manager, &QNetworkAccessManager::finished,this,&Weather::replyFinished);
    connect(manager2, &QNetworkAccessManager::finished,this,&Weather::replyFinished2);
    connect(manager3, &QNetworkAccessManager::finished,this,&Weather::replyFinished3);


    //2.往http://qt-project.org 网站发起请求  %E5%B9%BF%E5%B7%9E 广州
     manager->get(QNetworkRequest(QUrl("http://v2.alapi.cn/api/tianqi"
                                       "?token=oHSXnv0jZT8DbyTW&city=广州")));

     manager2->get(QNetworkRequest(QUrl("http://v2.alapi.cn/api/weather/forecast"
                                        "?token=oHSXnv0jZT8DbyTW&location=广州")));
}

void prase_today_weather(QNetworkAccessManager *manager3, Ui::Weather *ui, QString msg);
void prase_feature_weather(Ui::Weather *ui, QString msg);
void prase_lunar_day(Ui::Weather *ui, QString msg);

//today weather
void Weather::replyFinished(QNetworkReply *reply)
{
    //读取所有的网络数据
    QString  msg    =   reply->readAll();
    //qDebug() << "-----------today weather-------------" << msg;
    prase_today_weather(manager3,ui,msg);
}

//feature 4 day weather
void Weather::replyFinished2(QNetworkReply *reply)
{
    //读取所有的网络数据
    QString  msg    =   reply->readAll();
    //qDebug() << "-----------feature 4 day weather---------" << msg;
    prase_feature_weather(ui, msg);
}


//lunar day
void Weather::replyFinished3(QNetworkReply *reply)
{
    //读取所有的网络数据
    QString  msg    =   reply->readAll();
    //qDebug() << "---------------lunar day--------------" << msg;
    //qDebug() <<   msg;
    prase_lunar_day(ui, msg);
}

Weather::~Weather()
{
    delete ui;
}
//back main page
void Weather::on_pushButton_clicked()
{
    this->hide();
    this->parentWidget()->show();
}

//下拉框的内容改变了就触发
void Weather::on_comboBox_currentTextChanged(const QString &arg)
{
    city = arg;
    //qDebug() << arg;
    manager->get(QNetworkRequest(QUrl(QString("http://v2.alapi.cn/api/tianqi"
                                              "?token=oHSXnv0jZT8DbyTW&city=%1").arg(arg))));

    manager2->get(QNetworkRequest(QUrl(QString("http://v2.alapi.cn/api/weather/forecast"
                                       "?token=oHSXnv0jZT8DbyTW&location=%1").arg(arg))));

}




void prase_today_weather(QNetworkAccessManager *manager3, Ui::Weather *ui, QString msg)
{
    int end = msg.indexOf("rain");

    int start = msg.indexOf("update_time");
    msg = msg.mid(start, end-start);
    //qDebug() << msg;

    today_weather.update_time = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-15);


    start = msg.indexOf("weather");
    msg = msg.mid(start, end-start);
    today_weather.weather = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("temp");
    msg = msg.mid(start, end-start);
    today_weather.temp = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-8);

    start = msg.indexOf("min_temp");
    msg = msg.mid(start, end-start);
    today_weather.min_temp = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-12);

    start = msg.indexOf("max_temp");
    msg = msg.mid(start, end-start);
    today_weather.max_temp = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-12);

    start = msg.indexOf("wind");
    msg = msg.mid(start, end-start);
    today_weather.wind = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-8);

    start = msg.indexOf("wind_speed");
    msg = msg.mid(start, end-start);
    today_weather.wind_speed = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-14);

    start = msg.indexOf("wind_scale");
    msg = msg.mid(start, end-start);
    today_weather.wind_scale = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-17);

    //ui->label_temp->setNum(today_weather.temp.toInt());
    //QString tmp = QString("温度: %1°C").arg(today_weather.temp);
    //QString tmp = QString("温度:") + today_weather.temp;
    ui->label_temp->setText(QString("温度: %1°C").arg(today_weather.temp));
   // ui->label_temp->repaint();
    ui->label_temprange->setText(QString("%1 ~ %2°C").arg(today_weather.min_temp).arg(today_weather.max_temp));
    ui->label_weather->setText(today_weather.weather);
    ui->label_wind->setText(QString("%1 %2 %3/h").arg(today_weather.wind).arg(today_weather.wind_speed).arg(today_weather.wind_scale));
    ui->label_updatetime->setText(QString("更新时间: %1").arg(today_weather.update_time));

    today_weather.update_time.chop(9);
    manager3->get(QNetworkRequest(QUrl(QString("http://v2.alapi.cn/api/lunar"
                                              "?token=oHSXnv0jZT8DbyTW&date=%1").arg(today_weather.update_time))));

      //qDebug() << today_weather.update_time;
//    qDebug() << today_weather.weather;
//    qDebug() << today_weather.temp;
//    qDebug() << today_weather.temp.append("°C");
//    qDebug() << today_weather.min_temp;
//    qDebug() << today_weather.max_temp;
//    qDebug() << today_weather.wind;
//    qDebug() << today_weather.wind_speed;
//    qDebug() << today_weather.wind_scale;


}


void prase_feature_weather(Ui::Weather *ui, QString msg)
{
    //qDebug() << msg;
    //QString tmp;

    int end = msg.indexOf("]");

    int start = msg.indexOf("[")+2;
    msg = msg.mid(start, end-start);

    start = msg.indexOf("{");
    msg = msg.mid(start, end-start);


    start = msg.indexOf("cond_txt_d");
    msg = msg.mid(start, end-start);
    feature1_weather.weather = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-14) + " → ";

    start = msg.indexOf("cond_txt_n");
    msg = msg.mid(start, end-start);
    feature1_weather.weather += msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-14);

    start = msg.indexOf("date");
    msg = msg.mid(start, end-start);
    feature1_weather.update_time = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-8);

    start = msg.indexOf("tmp_max");
    msg = msg.mid(start, end-start);
    feature1_weather.max_temp = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("tmp_min");
    msg = msg.mid(start, end-start);
    feature1_weather.min_temp = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("wind_dir");
    msg = msg.mid(start, end-start);
    feature1_weather.wind = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-12);

    start = msg.indexOf("wind_sc");
    msg = msg.mid(start, end-start);
    feature1_weather.wind_scale = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("wind_spd");
    msg = msg.mid(start, end-start);
    feature1_weather.wind_speed = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-13);

    //qDebug() << "----------------------------------------";

    start = msg.indexOf("cond_txt_d");
    msg = msg.mid(start, end-start);
    feature2_weather.weather = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-14) + " → ";

    start = msg.indexOf("cond_txt_n");
    msg = msg.mid(start, end-start);
    feature2_weather.weather += msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-14);

    start = msg.indexOf("date");
    msg = msg.mid(start, end-start);
    feature2_weather.update_time = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-8);

    start = msg.indexOf("tmp_max");
    msg = msg.mid(start, end-start);
    feature2_weather.max_temp = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("tmp_min");
    msg = msg.mid(start, end-start);
    feature2_weather.min_temp = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("wind_dir");
    msg = msg.mid(start, end-start);
    feature2_weather.wind = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-12);

    start = msg.indexOf("wind_sc");
    msg = msg.mid(start, end-start);
    feature2_weather.wind_scale = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("wind_spd");
    msg = msg.mid(start, end-start);
    feature2_weather.wind_speed = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-13);

    //qDebug() << "----------------------------------------";

    start = msg.indexOf("cond_txt_d");
    msg = msg.mid(start, end-start);
    feature3_weather.weather = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-14) + " → ";

    start = msg.indexOf("cond_txt_n");
    msg = msg.mid(start, end-start);
    feature3_weather.weather += msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-14);

    start = msg.indexOf("date");
    msg = msg.mid(start, end-start);
    feature3_weather.update_time = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-8);

    start = msg.indexOf("tmp_max");
    msg = msg.mid(start, end-start);
    feature3_weather.max_temp = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("tmp_min");
    msg = msg.mid(start, end-start);
    feature3_weather.min_temp = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("wind_dir");
    msg = msg.mid(start, end-start);
    feature3_weather.wind = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-12);

    start = msg.indexOf("wind_sc");
    msg = msg.mid(start, end-start);
    feature3_weather.wind_scale = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("wind_spd");
    msg = msg.mid(start, end-start);
    feature3_weather.wind_speed = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-13);

    //qDebug() << "----------------------------------------";

    start = msg.indexOf("cond_txt_d");
    msg = msg.mid(start, end-start);
    feature4_weather.weather = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-14) + " → ";

    start = msg.indexOf("cond_txt_n");
    msg = msg.mid(start, end-start);
    feature4_weather.weather += msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-14);

    start = msg.indexOf("date");
    msg = msg.mid(start, end-start);
    feature4_weather.update_time = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-8);

    start = msg.indexOf("tmp_max");
    msg = msg.mid(start, end-start);
    feature4_weather.max_temp = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("tmp_min");
    msg = msg.mid(start, end-start);
    feature4_weather.min_temp = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("wind_dir");
    msg = msg.mid(start, end-start);
    feature4_weather.wind = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-12);

    start = msg.indexOf("wind_sc");
    msg = msg.mid(start, end-start);
    feature4_weather.wind_scale = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-11);

    start = msg.indexOf("wind_spd");
    msg = msg.mid(start, end-start);
    feature4_weather.wind_speed = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-13);

    //qDebug() << "----------------------------------------";


    ui->label_f1_date->setText(feature1_weather.update_time);
    ui->label_f1_range->setText(QString("%1 ~ %2°C").arg(feature1_weather.min_temp).arg(feature1_weather.max_temp));
    ui->label_f1_weather->setText(feature1_weather.weather);
    ui->label_f1_wind->setText(QString("%1 %2级 %3km/h").arg(feature1_weather.wind).arg(feature1_weather.wind_scale).arg(feature1_weather.wind_speed));

    ui->label_f2_date->setText(feature2_weather.update_time);
    ui->label_f2_range->setText(QString("%1 ~ %2°C").arg(feature2_weather.min_temp).arg(feature2_weather.max_temp));
    ui->label_f2_weather->setText(feature2_weather.weather);
    ui->label_f2_wind->setText(QString("%1 %2级 %3km/h").arg(feature2_weather.wind).arg(feature2_weather.wind_scale).arg(feature2_weather.wind_speed));

    ui->label_f3_date->setText(feature3_weather.update_time);
    ui->label_f3_range->setText(QString("%1 ~ %2°C").arg(feature3_weather.min_temp).arg(feature3_weather.max_temp));
    ui->label_f3_weather->setText(feature3_weather.weather);
    ui->label_f3_wind->setText(QString("%1 %2级 %3km/h").arg(feature3_weather.wind).arg(feature3_weather.wind_scale).arg(feature3_weather.wind_speed));

    ui->label_f4_date->setText(feature4_weather.update_time);
    ui->label_f4_range->setText(QString("%1 ~ %2°C").arg(feature4_weather.min_temp).arg(feature4_weather.max_temp));
    ui->label_f4_weather->setText(feature4_weather.weather);
    ui->label_f4_wind->setText(QString("%1 %2级 %3km/h").arg(feature4_weather.wind).arg(feature4_weather.wind_scale).arg(feature4_weather.wind_speed));

//    qDebug() << feature1_weather.weather;
//    qDebug() << feature1_weather.update_time;
//    qDebug() << feature1_weather.max_temp;
//    qDebug() << feature1_weather.min_temp;
//    qDebug() << feature1_weather.wind;
//    qDebug() << feature1_weather.wind_scale;
//    qDebug() << feature1_weather.wind_speed;
//    qDebug() << "----------------------------------------";
//    qDebug() << feature2_weather.weather;
//    qDebug() << feature2_weather.update_time;
//    qDebug() << feature2_weather.max_temp;
//    qDebug() << feature2_weather.min_temp;
//    qDebug() << feature2_weather.wind;
//    qDebug() << feature2_weather.wind_scale;
//    qDebug() << feature2_weather.wind_speed;
//    qDebug() << "----------------------------------------";
//    qDebug() << feature3_weather.weather;
//    qDebug() << feature3_weather.update_time;
//    qDebug() << feature3_weather.max_temp;
//    qDebug() << feature3_weather.min_temp;
//    qDebug() << feature3_weather.wind;
//    qDebug() << feature3_weather.wind_scale;
//    qDebug() << feature3_weather.wind_speed;
//    qDebug() << "----------------------------------------";
//    qDebug() << feature4_weather.weather;
//    qDebug() << feature4_weather.update_time;
//    qDebug() << feature4_weather.max_temp;
//    qDebug() << feature4_weather.min_temp;
//    qDebug() << feature4_weather.wind;
//    qDebug() << feature4_weather.wind_scale;
//    qDebug() << feature4_weather.wind_speed;
//    qDebug() << "----------------------------------------";


}

//解析农历时间
void prase_lunar_day(Ui::Weather *ui, QString msg)
{
    int end = msg.indexOf("is_today");

    int start = msg.indexOf("lunar_month_chinese");
    msg = msg.mid(start, end-start);
    //qDebug() << msg;

    //QString tmp;
    lunar_day.lunar_month = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-23);
    //qDebug() << tmp;


    start = msg.indexOf("lunar_day_chinese");
    msg = msg.mid(start, end-start);
    lunar_day.lunar_day = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-21);
    //qDebug() << tmp;

    start = msg.indexOf("gregorian_month");
    msg = msg.mid(start, end-start);
    lunar_day.gregorian_month = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-19);
    //qDebug() << tmp;

    start = msg.indexOf("gregorian_day");
    msg = msg.mid(start, end-start);
    lunar_day.gregorian_day = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-17);
    //qDebug() << tmp;

    start = msg.indexOf("week_no");
    msg = msg.mid(start, end-start);
    lunar_day.week_no = msg.mid(msg.indexOf(":")+1, 1);//msg.indexOf(",")
    //qDebug() << tmp;
    //qDebug() << msg;

    start = msg.indexOf("week_name");
    msg = msg.mid(start, end-start);
    lunar_day.week_name = msg.mid(msg.indexOf(":")+2, msg.indexOf(",")-13);
    //qDebug() << tmp;

    ui->label_today->setText(QString("%1月%2日 %3 农历%4%5")
     .arg(lunar_day.gregorian_month).arg(lunar_day.gregorian_day).arg(lunar_day.week_name).arg(lunar_day.lunar_month).arg(lunar_day.lunar_day));


     ui->label_day1->setText(week[(lunar_day.week_no.toInt()+1)%7]);
     ui->label_day2->setText(week[(lunar_day.week_no.toInt()+2)%7]);
     ui->label_day3->setText(week[(lunar_day.week_no.toInt()+3)%7]);
     ui->label_day4->setText(week[(lunar_day.week_no.toInt()+4)%7]);


//    qDebug() << lunar_day.lunar_month;
//    qDebug() << lunar_day.lunar_day;
//    qDebug() << lunar_day.gregorian_month;
//    qDebug() << lunar_day.gregorian_day;
//    qDebug() << lunar_day.week_no;
//    qDebug() << lunar_day.week_name;




}






















































