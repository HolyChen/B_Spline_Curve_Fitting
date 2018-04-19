//
// Created by haochuanchen on 18-4-6.
//

#ifndef B_SPLINE_CURVEIO_H
#define B_SPLINE_CURVEIO_H

#include <QFile>
#include <QtCore/QTextStream>

#include <type_traits>

#include "../base_type/utility.h"
#include "../Curve.h"

template <typename>
struct Curve;

template <typename _CurvePoint>
class CurveIO
{
public:
    using _Pt = _CurvePoint;
    using _Dt = typename _Pt::_Dt;
    using _Ct = Curve<_Pt>;

private:
    QString m_filename;

public:
    void read_from_file(_Ct& curve, const QString& filename)
    {
        auto& vertices = curve.get_vertices();

        this->m_filename = filename;
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            QString line = in.readLine();
            if(line.isNull())
            {
                file.close();
                return;
            }

            _Pt point;
            while (!line.isNull())
            {
                QStringList slist = line.split(" ");
                if(slist.size() > 3)
                {
                    if(!slist.at(0).compare("v"))
                    {
                        point.vertex.x = slist.at(1).toDouble();
                        point.vertex.y = slist.at(2).toDouble();
                        point.vertex.z = slist.at(3).toDouble();
                        vertices.push_back(point);
                    }
                }
                line = in.readLine();
            }
            file.close();
        }
    }
};


#endif //B_SPLINE_CURVEIO_H
