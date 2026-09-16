#include "write_csv.h"
#include <fstream>
#include <iostream>

// write out another csv file for Panel. Finish designing double csv file exporting.
bool writeCSV_NACA(std::vector<Vec2> & points, const std::string & point_csv)
{
    std::ofstream file {point_csv};

    if (!file)
    {
        return false;
    }

    file << "x,y" << '\n';

    for (const Vec2 & p : points)
    {
        file << p.x << ',' << p.y << '\n';
    }

    return true;
}


bool writeCSV_Panel(std::vector<Panel> & panels, const std::string & panel_csv)
{
    std::ofstream panel_file {panel_csv};
    if (!panel_file)
    {
        return false;
    }
    panel_file << "start_x,start_y,end_x,end_y,mid_x,mid_y,tan_x,tan_y,nor_x,nor_y,length" << '\n';

    for (const Panel & pa: panels)
    {
        panel_file << pa.start.x << ',' << pa.start.y << ','
                   << pa.end.x << ',' << pa.end.y << ','
                   << pa.midpoint.x << ',' << pa.midpoint.y << ','
                   << pa.tangent.x << ',' << pa.tangent.y << ','
                   << pa.normal.x << ',' << pa.normal.y << ',' 
                   << pa.length << '\n';


    }
    return true;
}


bool writeCSV_Polar (std::vector<double> & angles, std::vector<double> & Cl, const std::string & polar_csv)
{
    std::ofstream polar_file {polar_csv};
    if (angles.size() != Cl.size()) return false;
    polar_file << "angle_deg,Cl" << '\n';

    for (int k = 0; k < static_cast<int>(Cl.size()); ++k)
    {
        polar_file << angles[k] << ',' << Cl[k] << '\n';
    }
    return true;
}