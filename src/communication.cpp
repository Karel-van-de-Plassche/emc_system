#include "emc/communication.h"

#include <ros/node_handle.h>

#include <geometry_msgs/Twist.h>

namespace emc
{

// ----------------------------------------------------------------------------------------------------

Communication::Communication()
{
}

// ----------------------------------------------------------------------------------------------------

Communication::~Communication()
{
}

// ----------------------------------------------------------------------------------------------------

void Communication::init()
{
    ros::NodeHandle nh;
    nh.setCallbackQueue(&cb_queue_);
    sub_laser_ = nh.subscribe<sensor_msgs::LaserScan>("/pico/laser", 1, &Communication::laserCallback, this);
    pub_base_ref_ = nh.advertise<geometry_msgs::Twist>("/pico/base/reference", 1);
}

// ----------------------------------------------------------------------------------------------------

bool Communication::readLaserData(LaserData& scan)
{
    laser_msg_.reset();
    cb_queue_.callAvailable();

    if (!laser_msg_)
        return false;

    scan.range_max = laser_msg_->range_max;
    scan.ranges = laser_msg_->ranges;
    return true;
}

// ----------------------------------------------------------------------------------------------------

void Communication::sendBaseReference(double vx, double vy, double va)
{
    geometry_msgs::Twist ref;
    ref.linear.x = vx;
    ref.linear.y = vy;
    ref.angular.z = va;

    pub_base_ref_.publish(ref);
}

// ----------------------------------------------------------------------------------------------------

void Communication::laserCallback(const sensor_msgs::LaserScanConstPtr& msg)
{
    laser_msg_ = msg;
}

} // end namespace emc

