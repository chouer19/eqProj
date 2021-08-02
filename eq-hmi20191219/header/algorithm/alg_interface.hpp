#ifndef ALG_INTERFACE_H_
#define ALG_INTERFACE_H_

/*
 * =====================================================================
 * Include files (NOT RECOMMENDED, if needed SHOULD be out of "extern C")
 * =====================================================================
 */
#include "header/system/pub_types.h"


/*
 * =====================================================================
 * Global pre-processor symbols/macros ('#define')
 * =====================================================================
 */

enum CON_SWITCH
{
	CON_OFF = 0,
	CON_ON
};

enum DIRECTION
{
    FORWARD=0,
    BACKWARD=1
};

struct path_t
{
    path_t()
        :x(0)
        ,y(0)
        ,z(0)
        ,distance(0)
        ,heading(0)
        ,tanYaw(0)
        ,curvature(0)
        ,vspd(0)
        ,accel(0)
    {}
    path_t(double _x, double _y, double _z,double _distance,double _heading,double _ty, double _curv, double _v, double _a = 0)
        :x(_x)
        ,y(_y)
        ,z(_z)
        ,distance(_distance)
        ,heading(_heading)
        ,tanYaw(_ty)
        ,curvature(_curv)
        ,vspd(_v)
        ,accel(_a)
    {}
    double x;
    double y;
    double z;
    double distance;
    double heading;
    double tanYaw;
    double curvature;
    double vspd;
    double accel;
};

struct path_pt_t
{
    path_pt_t()
    :time_stamp(0)
    {
        pt.clear();
    }
    path_pt_t(const size_t &_t, const vector<path_t> &_path_list)
    :time_stamp(_t)
    ,pt(_path_list)
    {}
    size_t time_stamp;
    vector<path_t> pt;
};


/*
 * =====================================================================
 * Global type definitions ('typedef')
 * =====================================================================
 */

/* lonctrl */
DeclareBasedInterface(ILONCTRL, TASK)
    ILONCTRL()
    :TASK(pub_time, 50)
     {}
    ILONCTRL(CCTIME *_time, uint8_t _period)
    :TASK(_time, _period)
    {}
    virtual void install() = 0;
    virtual void setToControl(bool _to_con) = 0;
    virtual void setDirection(DIRECTION dir)=0;
    virtual void setAimAcc(double _aimAcc)=0;
    virtual err_t error() = 0 ;
EndInterface

/* latctrl */
DeclareBasedInterface(ILATCTRL, TASK)
    ILATCTRL()
    :TASK(pub_time, 50)
     {}
    ILATCTRL(CCTIME *_time, uint8_t _period)
    :TASK(_time, _period)
    {}
    virtual void install() = 0;
    virtual void setToControl(bool _to_con) = 0;
    virtual void setDirection(DIRECTION dir)=0;
    virtual err_t error() = 0 ;
EndInterface

/* Speed plan */
DeclareBasedInterface(ISPEEDPLAN, TASK)
    ISPEEDPLAN()
    :TASK(pub_time, 50)
    {}
    ISPEEDPLAN(CCTIME *_time, uint8_t _period)
        :TASK(_time, _period)
    {}
    virtual void install() = 0;
    virtual bool active() = 0;
    virtual void deactive() = 0;
    virtual bool activated() = 0;
    virtual uint8_t error() = 0;
    virtual void setPath(path_pt_t _path)=0;

EndInterface

/* Path plan */
DeclareBasedInterface(IPATHPLAN, TASK)
    IPATHPLAN()
    :TASK(pub_time, 50)
    {}
    IPATHPLAN(CCTIME *_time, uint8_t _period)
        :TASK(_time, _period)
    {}
    virtual void install() = 0;
    virtual bool active() = 0;
    virtual void deactive() = 0;
    virtual bool activated() = 0;
    virtual uint8_t error() = 0;

EndInterface

/**************************algorithm Interface******************************/
class ALGORITHM
{
public:
    ALGORITHM();
    virtual ~ALGORITHM();

    ILATCTRL *latctrl;
    ILONCTRL *lonctrl;
    ISPEEDPLAN *speed;
    IPATHPLAN *path;

};

/*
 * =====================================================================
 * Global constant declarations ('extern', definiton&comment in C source)
 * =====================================================================
 */


/*
 * =====================================================================
 * Global variable declarations ('extern', definition&comment in C source)
 * =====================================================================
 */



/*
 * =====================================================================
 * Global function prototypes ('extern', definition&comment in C source)
 * =====================================================================
 */


#endif /* ALG_INTERFACE_H_ */
