#include "arucodetector.h"
//#include "qdebug.h"
//#include <QMatrix3x3>
//#include <QQuaternion>
#include <opencv2/calib3d.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <opencv2/objdetect/aruco_dictionary.hpp>
//#include <QLabel>

cv::Mat eulerToRotationMatrix(double roll, double pitch, double yaw) {
    cv::Mat R_x = (cv::Mat_<double>(3,3) <<
                   1, 0, 0,
                   0, cos(roll), -sin(roll),
                   0, sin(roll), cos(roll));

    cv::Mat R_y = (cv::Mat_<double>(3,3) <<
                   cos(pitch), 0, sin(pitch),
                   0, 1, 0,
                   -sin(pitch), 0, cos(pitch));

    cv::Mat R_z = (cv::Mat_<double>(3,3) <<
                   cos(yaw), -sin(yaw), 0,
                   sin(yaw), cos(yaw), 0,
                   0, 0, 1);

    return R_z * R_y * R_x;
}
/*
QImage matToQImage(const cv::Mat& mat) {
    if (mat.empty()) {
        return QImage();
    }

    // Конвертируем в RGB если нужно
    cv::Mat rgbMat;
    if (mat.channels() == 1) {
        cv::cvtColor(mat, rgbMat, cv::COLOR_GRAY2RGB);
    } else if (mat.channels() == 3) {
        cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
    } else if (mat.channels() == 4) {
        cv::cvtColor(mat, rgbMat, cv::COLOR_BGRA2RGBA);
    } else {
        // Для других форматов конвертируем в RGB
        mat.convertTo(rgbMat, CV_8UC3);
        if (rgbMat.channels() == 3) {
            cv::cvtColor(rgbMat, rgbMat, cv::COLOR_BGR2RGB);
        }
    }

    // Создаем QImage и копируем данные
    QImage image(rgbMat.data,
                 rgbMat.cols,
                 rgbMat.rows,
                 static_cast<int>(rgbMat.step),
                 rgbMat.channels() == 4 ? QImage::Format_RGBA8888 : QImage::Format_RGB888);

    return image.copy(); // Важно: создаем копию!
}
*/

ArucoDetector::ArucoDetector(QObject *parent) : QObject(parent),
    m_x(0), m_y(0), m_z(0), m_yaw(0)
{
}

void ArucoDetector::holdOnMarker(bool hold)
{
    m_holdOnMarker = hold;
}

void ArucoDetector::detectMarkers(const cv::Mat &image)
{
    std::vector<std::vector<cv::Point2f>> rejectedCandidates;
    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_1000);
    cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
    cv::aruco::ArucoDetector detector(dictionary, detectorParams);
    detector.detectMarkers(image, m_markerCorners, m_arucoIds, rejectedCandidates);
    m_qListArucoIds.clear();
    for (size_t i = 0; i < m_arucoIds.size(); ++i) {
        m_qListArucoIds << m_arucoIds[i];
    }

    if (m_arucoIds.size()) {
        ///-------------------------------------
        /*
        cv::Mat cameraMatrix = (cv::Mat_<double>(3,3) <<
                        498.7021006159341, 0, 319.4959725904311,
                        0, 499.1050288853072, 242.0745930597295,
                        0, 0, 1);
        //cv::Mat distCoeffs = (cv::Mat_<double>(1,5) << 0.1566133379751138, -0.163405759544564, -0.001003945105023733, 0.004925577956378657, -0.4052163095421999);
        cv::Mat distCoeffs = (cv::Mat_<double>(1,5) << 0.0, 0.0, 0.0, 0.0, 0.0);
        std::vector<cv::Point2f> imagePoints;
        std::vector<cv::Point3f> objPoints;
        for (size_t i = 0; i < arucoIds.size(); ++i) {
            float markerSize = 0.2;
            float halfSize = markerSize / 2.0f;
            float offsetX = (arucoIds[i] % 4) * 0.4;
            float offsetY = (arucoIds[i] / 4) * 0.4;
            imagePoints.push_back(markerCorners[i][0]);
            imagePoints.push_back(markerCorners[i][1]);
            imagePoints.push_back(markerCorners[i][2]);
            imagePoints.push_back(markerCorners[i][3]);
            objPoints.push_back(cv::Point3f(-halfSize + offsetX, halfSize + offsetY, 0));
            objPoints.push_back(cv::Point3f(halfSize + offsetX, halfSize + offsetY, 0));
            objPoints.push_back(cv::Point3f(halfSize + offsetX, -halfSize + offsetY, 0));
            objPoints.push_back(cv::Point3f(-halfSize + offsetX, -halfSize + offsetY, 0));
        }
        cv::Mat rvec, tvec;
        cv::solvePnP(objPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);

        // Расстояние = длина вектора трансляции
        //double distance = cv::norm(tvec);
        //qDebug()<< " X: " << tvec.at<double>(0)
        //        << " Y: " << tvec.at<double>(1)
        //        << " Z: " << tvec.at<double>(2);
        double x = -tvec.at<double>(0);
        double y = -tvec.at<double>(1);
        double z = tvec.at<double>(2);

        cv::Mat r;
        Rodrigues(rvec, r);
        double yaw = atan2(r.at<double>(1,0), r.at<double>(0,0));

        emit coordsChanged(x, y, z, yaw);


        // 1. Получаем положение камеры относительно маркера из solvePnP
        cv::Mat R_cam_marker;
        cv::Rodrigues(rvec, R_cam_marker);
        cv::Mat P_cam_in_marker = -R_cam_marker.t() * tvec;

        // 2. Создаем матрицу компенсации наклона из IMU
        // Это поворот, который "выравнивает" систему координат камеры
        cv::Mat R_compensation = eulerToRotationMatrix(-mavlinkInterface->roll(), -mavlinkInterface->pitch(), yaw);

        // 3. Применяем компенсацию наклона
        cv::Mat P_corrected = R_compensation * P_cam_in_marker;
        double _x = P_corrected.at<double>(0);
*/
        ///-------------------------------------
        /*
        float total_tilt = qDegreesToRadians(5);
        if (mavlinkInterface->roll() < -total_tilt || mavlinkInterface->roll() > total_tilt ||
            mavlinkInterface->pitch() < -total_tilt || mavlinkInterface->pitch() > total_tilt)
            return;
            */
        //QList<Altitude> altitudeList;
        QList<qreal> xList;
        QList<qreal> yList;
        QList<qreal> zList;
        QList<qreal> yawList;
        for (size_t i = 0; i < m_arucoIds.size(); ++i) {

#ifdef __arm__
            cv::Mat cameraMatrix = (cv::Mat_<double>(3,3) <<
                                    498.7021006159341, 0, 319.4959725904311,
                                    0, 499.1050288853072, 242.0745930597295,
                                    0, 0, 1);
            //cv::Mat distCoeffs = (cv::Mat_<double>(1,5) << 0.1566133379751138, -0.163405759544564, -0.001003945105023733, 0.004925577956378657, -0.4052163095421999);
            cv::Mat distCoeffs = (cv::Mat_<double>(1,5) << 0.0, 0.0, 0.0, 0.0, 0.0);
            float markerSize = 0.15;
#else
            cv::Mat cameraMatrix = (cv::Mat_<double>(3,3) <<
                                    948.9911023478246, 0, 595.9509910458424,
                                    0, 949.3816178985988, 341.9459545416092,
                                    0, 0, 1);
            cv::Mat distCoeffs = (cv::Mat_<double>(1,5) << -0.1924993197173277, 0.163548107537344, -0.008001373003260755, -0.007208196517934148, 0.6201743976210183);
            float markerSize = 0.0565;
#endif
            std::vector<cv::Point3f> objPoints;
            // 3D точки маркера в системе координат маркера
            //float markerSize = 0.2;
            //float offset = 0.4;
            //int side = 4;
            float offset = 0.3;
            int side = 10;

            float halfSize = markerSize / 2.0f;
            objPoints.push_back(cv::Point3f(-halfSize, halfSize, 0));
            objPoints.push_back(cv::Point3f(halfSize, halfSize, 0));
            objPoints.push_back(cv::Point3f(halfSize, -halfSize, 0));
            objPoints.push_back(cv::Point3f(-halfSize, -halfSize, 0));

            cv::Vec3d rvec, tvec;
            // Находим позицию маркера
            cv::solvePnP(objPoints, m_markerCorners[i], cameraMatrix, distCoeffs, rvec, tvec);

            cv::Mat r_mat;
            cv::Rodrigues(rvec, r_mat);

            /*
            qreal roll = asin(-r_mat.at<double>(2,0));
            qreal pitch = atan2(r_mat.at<double>(2,1), r_mat.at<double>(2,2));
            qreal yaw = atan2(r_mat.at<double>(1,0), r_mat.at<double>(0,0));


            static quint64 timer = QDateTime::currentDateTime().toMSecsSinceEpoch();
            if (QDateTime::currentDateTime().toMSecsSinceEpoch() - timer >= 200) {
            qDebug() << "roll" << roll * 180 / M_PI
                     << "pitch" << pitch * 180 / M_PI
                     << "yaw" << yaw * 180 / M_PI << '\n';
            }
            */

            /*
            r_mat = r_mat.t();
            QMatrix3x3 qRmat;
            for (quint8 i = 0; i < 3; ++i) {
                for (quint8 j = 0; j < 3; ++j) {
                    qRmat(i, j) = r_mat.at<qreal>(i, j);
                }
            }
            QQuaternion quaternion = QQuaternion::fromRotationMatrix(qRmat);
            QVector3D euler = quaternion.toEulerAngles();
            QVector3D qTvec(tvec[0], tvec[1], tvec[2]);
            QVector3D pos = quaternion.rotatedVector(qTvec);
            */

            // Простой вариант
            xList << -tvec[0] + (m_arucoIds[i] % side) * offset;
            yList << tvec[1] + 3 - (m_arucoIds[i] / side) * offset;
            zList << tvec[2];

            // Поворот через rvec
            //cv::Mat t_mat = -r_mat.t() * tvec;
            //xList << pos.x() + (m_arucoIds[i] % side) * offset;
            //yList << pos.y() + (m_arucoIds[i] / side) * offset;
            //zList << pos.z();

            yawList << atan2(r_mat.at<double>(1,0), r_mat.at<double>(0,0));

            /*
            // Поворот с IMU
            cv::Mat r;
            cv::Rodrigues(rvec, r);
            double yaw = atan2(r.at<double>(1,0), r.at<double>(0,0));
            cv::Mat r_correct = eulerToRotationMatrix(-mavlinkInterface->roll(), -mavlinkInterface->pitch(), yaw);

            cv::Mat t = -r_correct.t() * cv::Mat(tvec);

            altitude.x = t.at<qreal>(0) + (arucoIds[i] % 4) * 0.4;
            altitude.y = t.at<qreal>(1) + (arucoIds[i] / 4) * 0.4;
            altitude.z = t.at<qreal>(2);
            altitude.yaw = yaw;

            altitudeList << altitude;
            */
        }
        /*
        // среднее значение
        qreal sum_x = 0;
        qreal sum_y = 0;
        qreal sum_z = 0;
        qreal sum_yaw = 0;
        for (const Altitude &altitude : altitudeList) {
            sum_x += altitude.x;
            sum_y += altitude.y;
            sum_z += altitude.z;
            sum_yaw += altitude.yaw;
        }
        qreal x = sum_x / altitudeList.size();
        qreal y = sum_y / altitudeList.size();
        qreal z = sum_z / altitudeList.size();
        qreal yaw = sum_yaw / altitudeList.size();
        */
        // медиана
        const qsizetype size = m_arucoIds.size();
        /*
        qreal x = 0;
        qreal y = 0;
        qreal z = 0;
        qreal yaw = 0;
        */
        std::sort(xList.begin(), xList.end());
        std::sort(yList.begin(), yList.end());
        std::sort(zList.begin(), zList.end());
        std::sort(yawList.begin(), yawList.end());
        if (size % 2 == 0) {
            m_x = (xList.at(size / 2 - 1) + xList.at(size / 2)) / 2.f;
            m_y = (yList.at(size / 2 - 1) + yList.at(size / 2)) / 2.f;
            m_z = (zList.at(size / 2 - 1) + zList.at(size / 2)) / 2.f;
            m_yaw = (yawList.at(size / 2 - 1) + yawList.at(size / 2)) / 2.f;
        } else {
            m_x = xList.at(size / 2);
            m_y = yList.at(size / 2);
            m_z = zList.at(size / 2);
            m_yaw = yawList.at(size / 2);
        }
        ///-------------------------------------
    } else {
        //mavlinkInterface->align();
        //mavlinkInterface->setThrottlePWM(mavlinkInterface->throttlePWM() - 10);
        m_x = 0;
        m_y = 0;
        m_z = 0;
        m_yaw = 0;
    }
}

void ArucoDetector::drawDetectMarkers(cv::Mat &image)
{
    cv::aruco::drawDetectedMarkers(image, m_markerCorners, m_arucoIds);
}
