#include "stdio.h"
#include "Math.h"
#include "WPILib.h"

class Robot: public IterativeRobot {
	Talon left, right, frontarm, ballshooter; //initiates pins for talon actuators
	Spark liftleft, liftright; //initiates pins for spark actuators
	LiveWindow *lw; //Allows Test mode on robot with controller+computer
	RobotDrive myRobot; //RobotDrive controls two Pins
	Joystick xbox;//1st controller
	Joystick xbox2;//2nd controller
	Timer time; //This is a WPILib timer for autonomus
	float leftFollow = 0.0F, rightFollow = 0.0F, frontarmFollow = 0.0F, speedLimit = 0.75F,
			liftreturn = 0.0f, leftlift = 0.0F, leftright = 0.0F, liftdown = 0.0F ;
	//Floats are rational numbers that can be manipulated for a numerous amount of purposes

public:
	Robot() :
			left(0), right(1), frontarm(2),ballshooter(3),liftright(4), liftleft (5), myRobot(left, right), xbox(0), logitech(1), time (),leftFollow(0), rightFollow(0), frontarmFollow(0)
	{ //Robot() is a Constructor, primary function in the class, from here all variables can be initialized
		lw = LiveWindow::GetInstance();
//Likewise, variables can be initiated within the function, sometimes this is done because these variables require a method that cannot be done beforehand
		myRobot.SetExpiration(0.1);
//myRobot was initiated before hand, but another "part" of it was setup here. The Expiration is a check the myRobot class runs to check for the perscribed PWM channels accessibility
		CameraServer::GetInstance()->SetQuality(30); //The actual intialization of the Camera object was done outside of this class, by the FRC software, so don't worry about that
		CameraServer::GetInstance()->SetSize(100);//These three lines access the Camera Server at setup the Camera's Quality, Viewport Size, and tell the camera to begin
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");//The Camera is USB enabled and attached to the Roborio, the driver station has an option to access each USB port, the Camera was located on the HW USB Port
	}
private:
	//AnalogInput ai = new AnalogInput(0);
	void AutonomousInit() { //Function that initializes any variables in the autonomous stage, runs once
		myRobot.SetSafetyEnabled(false);
//Turns off the robot safety feature, it's really not necessary outside of the TeleOp phase, this just checks for if there are too many inputs/outputs at any point in time
		time.Reset(); //These setup the timer by reseting it and starting it.
		time.Start();
	}
	//The following autonomous periodic functions are legacy code, these where what where inplace before the AutonomousTemplate was created
	void AutonomousPeriodic_1() { //GATE
		float timeFollow = time.Get();
		if (timeFollow < 5.0) {
			myRobot.TankDrive(1,1);
		} else if (timeFollow >= 5.0 && timeFollow < 7.0){
			frontarm.SetSpeed(.25);
			myRobot.TankDrive(1,1);
		} else if (timeFollow >= 7.0 && timeFollow < 10.0){
			frontarm.SetSpeed(0);
			myRobot.TankDrive(1,1);
		}
	}

	void AutonomousPeriodic_2() { //hit
		float timeFollow = time.Get();
		if (timeFollow < 5.0) {
			myRobot.TankDrive(1,1);
			frontarm.SetSpeed(-.5);
		} else if (timeFollow >= 5.0 && timeFollow < 7.0){
			frontarm.SetSpeed(2);
			myRobot.TankDrive(1,1);
		}
	}

	void AutonomousPeriodic_3() { //Timer use test, Push forward
		float timeFollow = time.Get();
		if (timeFollow < 5.0) {
			myRobot.TankDrive(1,1);
			frontarm.SetSpeed(-.5);
		}	else if (timeFollow >= 5.0 && timeFollow < 7.0){
			frontarm.SetSpeed(2);
		} else if (timeFollow >= 7.0 && timeFollow < 10.0){
			myRobot.TankDrive(1,1);
		}
	}



	void TeleopInit() { //Like the AutonomousInit, the TeleOp init initializes functions, this runs after the AutonomousPeriodic period and before the 2:30 TeleOpPeriodic time
		myRobot.SetSafetyEnabled(true);
//Turns back on the robot safety
		time.Reset(); //Resets the timer incase necessary, or incase the autonomous period is run again, this occurs alot in testing and if you don't reset the timer, the robot may not perform properly
		time.Start();
		leftFollow = 0; //Initializes two of the floats from the beginning of the program which where utilized for coasting (more on that later)
		rightFollow = 0;
		CameraServer::GetInstance()->SetQuality(50);
		//the camera name (ex "cam0") can be found through the roborio web interface
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
	}

	void TeleopPeriodic() { //Periodic Tele-Operation function, this function runs as a loop for the period of the TeleOp portion of the competition
		float left = xbox.GetRawAxis(1), right = xbox.GetRawAxis(5), frontarmAxis;//Initializes more floats, but the left and right floats get inputs from the xbox joystick, they recieve raw inputs form Axis 1 nad 5, these are the Y-Axis on each of the two "joysticks" on the xbox controller
		float liftup = logitech.GetRawAxis(1);
		//float liftdown = xbox.GetRawAxis(3);
		float ballshooterSpeed = xbox.GetRawButton(7);
		float frontarmSpeed =xbox.GetRawButton(6);

		//float lifting = liftdown + liftup;
		bool ballsuckerin = xbox.GetRawButton(4);
		bool ballsuckerout = xbox.GetRawButton(1);
		//float printf (xbox.GetRawAxis(0));
		//liftright.SetSpeed(liftdown);
		//liftleft.SetSpeed(liftdown);

		//float ballshooterSpeed = xbox.GetRawButton(4);
		bool ballsucker = xbox.GetRawButton(2);
		bool frontarmUp =logitech.GetRawButton(5); //changed by kemal, normally 5
		bool frontarmDown = logitech.GetRawButton(3); //changed by kemal, normally 5
		//float printf (xbox.GetRawAxis(0));
		//liftleft.SetSpeed(liftdown*-1);
		//liftright.SetSpeed(liftdown*-1);
		liftleft.SetSpeed(liftup);
		liftright.SetSpeed(liftup);
		ballshooter.SetSpeed(ballshooterSpeed);


		frontarmAxis = frontarmSpeed; //Math, not much to explain here.
		leftFollow += (left - leftFollow) / 2; //+= adds the following values ((left - leftFollow)/2) to the former value (leftFollow)
		rightFollow += (left - leftFollow) / 2; //Again
		frontarmFollow += (frontarmAxis - frontarmFollow) / 2; //Agaaaaaaaaaaain  ITS YUUUGEEE
		//myRobot.TankDrive(-leftFollow*speedLimit, -rightFollow*speedLimit);
		myRobot.TankDrive(-left/* * 0.75*/, -right);
		//The inputs are directly recieved from the xbox controller, if coasting where to be used variables leftFollow and rightFollow would have been accessed, the *0.75 was an adjustment for some physical complications that we where having with the robot prior to the competition
		if (abs(left - leftFollow) < 0.01F)
{ //More logical checks, these are inplace to make sure the coasting values do not exceed maximum (Fun Fact: When they DID exceed maximum (before this was implemented) the robot would spin uncontrollabily)
			leftFollow = left;
		}
		if (abs(right - rightFollow) < 0.01F) {
			rightFollow = right;
		}
		if (abs(frontarmAxis - frontarmFollow) < 0.01F) {
			frontarmFollow = frontarmAxis;
		}
		if (leftFollow > 1 || leftFollow < -1) {
			leftFollow = 0;
		}
		if (rightFollow > 1 || rightFollow < -1) {
			rightFollow = 0;
		}
		if (frontarmFollow > 1 || frontarmFollow < -1) {
			frontarmFollow = 0;
		}
		frontarm.SetSpeed(frontarmFollow * 0.55F); //This is used to control the frontarm wheel the same type of opperation follows suite with the chain, the PWM channel is used to set the speed of the Talon which sends a pulse to the motor, causing it to move at a certain speed, that speed is a value from 0% to 100% controlled here between the values of 0 to 1
		//float up = xbox.GetRawAxis(2), down = xbox.GetRawAxis(3); //What is that strange value in the SetSpeed function for the motor, well, each button when pushed returns a value of 1, or 100%, and when not pushed returns a value of 0.
		//This means that while down is pushed the value is 0-1 = -1, at which point the motor will move in one direction consistently, if the up button where pushed then it would move at 1-0 = 1, the opposite direction




	}

	void TestPeriodic() { //For the test function to be used, begins the LiveWindow feed, once this is initialized on the driver station the connected computer can make use of the SmartDashboard program to control the robot
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot); //WPILib function to flag the Robot() class as the primary class for therobot, not much more to tell about it.
