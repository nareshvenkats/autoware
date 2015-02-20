#include <ros/ros.h>

#include <nav_msgs/Path.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <lane_follower/lane.h>

static constexpr uint32_t SUBSCRIBE_QUEUE_SIZE = 1000;

static constexpr uint32_t ADVERTISE_QUEUE_SIZE = 1000;
static constexpr bool ADVERTISE_LATCH = false;

static double config_velocity = 40; // Unit: km/h

static ros::Publisher pub_velocity;
static ros::Publisher pub_ruled;
static ros::Publisher pub_stop;

static uint32_t waypoint_count;

static void lane_waypoint_callback(const nav_msgs::Path& msg)
{
	std_msgs::Header header;
	header.stamp = ros::Time::now();
	header.frame_id = "/map";

	visualization_msgs::MarkerArray velocities;
	visualization_msgs::Marker velocity;
	velocity.header = header;
	velocity.ns = "velocity";

	if (waypoint_count > 0) {
		velocity.action = visualization_msgs::Marker::DELETE;
		for (uint32_t i = 0; i < waypoint_count; ++i) {
			velocity.id = i;
			velocities.markers.push_back(velocity);
		}
		pub_velocity.publish(velocities);
	}

	velocity.id = 0;
	velocity.action = visualization_msgs::Marker::ADD;
	velocity.lifetime = ros::Duration();
	velocity.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
	velocity.scale.z = 0.4;
	velocity.color.r = 1;
	velocity.color.a = 1;

	std::ostringstream ostr;
	ostr << std::fixed << std::setprecision(0) << config_velocity
	     << "km/h";
	velocity.text = ostr.str();

	lane_follower::lane ruled;
	ruled.header = header;

	lane_follower::waypoint waypoint;
	waypoint.pose.header = header;
	waypoint.twist.header = header;
	waypoint.pose.pose.orientation.w = 1;

	for (const geometry_msgs::PoseStamped& posestamped : msg.poses) {
		velocity.pose.position = posestamped.pose.position;
		velocity.pose.position.z += 0.2;
		velocities.markers.push_back(velocity);
		++velocity.id;

		waypoint.pose.pose.position = posestamped.pose.position;
		waypoint.twist.twist.linear.x = config_velocity / 3.6;
		ruled.waypoints.push_back(waypoint);
	}

	waypoint_count = velocity.id;

	pub_velocity.publish(velocities);
	pub_ruled.publish(ruled);

	lane_follower::lane stop;
	pub_stop.publish(stop);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "lane_rule");

	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("lane_waypoint",
					  SUBSCRIBE_QUEUE_SIZE,
					  lane_waypoint_callback);

	pub_velocity = n.advertise<visualization_msgs::MarkerArray>(
		"waypoint_velocity",
		ADVERTISE_QUEUE_SIZE,
		ADVERTISE_LATCH);
	pub_ruled = n.advertise<lane_follower::lane>(
		"ruled_waypoint",
		ADVERTISE_QUEUE_SIZE,
		ADVERTISE_LATCH);
	pub_stop = n.advertise<lane_follower::lane>(
		"stop_waypoint",
		ADVERTISE_QUEUE_SIZE,
		ADVERTISE_LATCH);

	ros::spin();

	return 0;
}
