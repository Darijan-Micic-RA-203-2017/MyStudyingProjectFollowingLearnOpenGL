// We used several preprocessor directives at the top of the header file. Using these little lines of code
// informs your compiler to only include and compile this header file if it hasn't been included yet, even if
// multiple files include the camera header. This prevents linking conflicts.
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

// Fly-like camera.
class Camera
{
private:
	// This vector acts as insurance that however we move, camera keeps looking straight ahead.
	// Math's explained below. In 2. thing we need to manually create LookAt matrix - "camera's direction":
	// glm::vec3 cameraTarget = cameraPosition + cameraFront;
	// glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget) = glm::normalize(-cameraFront);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::vec3(0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f);

	// We present camera's movement with Euler angles - 3 values that can represent any rotation in a 3D space.
	// We will use first two of Eurler angles: pitch (top/bottom, movement around a fixed x-axis) and yaw
	// (left/right, movement around a fixed y-axis). We won't calculate third's Euler angle, roll (movement around
	// a fixed z-axis), because we won't rotate camera like a mobile phone, bending it to left or right or turning
	// it upside-down.
	float pitch = 0.0f;
	// Yaw is initialized to -90.0f, because a 0.0f value would result in "camera's direction" vector pointing to
	// the right, towards the positive z-axis (on the xz plane). We don't want that, because we set up our camera
	// to look away from user, towards the negative z-axis. Therefore, we initialize yaw Euler angle with a
	// negative value, which means clockwise rotation.
	float yaw = -90.0f;
public:
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	float fov = 45.0f;

	Camera(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 upVector)
	{
		this->cameraPosition = cameraPosition;
		this->cameraFront = cameraFront;
		this->upVector = upVector;

		updateCameraCoordinateSystem();
	}

	// Utility function for calculating all vectors required to create the camera's coordinate system.
	void updateCameraCoordinateSystem()
	{
		// Calculate ACTUAL camera's direction vector, the result of subtracting camera's position from camera's
		// target (thus visually ending at camera's target, minuend of subtraction).
		glm::vec3 direction = glm::vec3(0.0f);
		direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		direction.y = sin(glm::radians(pitch));
		direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(direction);

		// "Camera's direction". It's a bad name, because we actually need the direction TO camera.
		// The camera's direction is counted by subtracting camera's position from the camera's target. However,
		// the camera in the OpenGL by convention points towards the negative z-axis and we want the z-axis in the
		// view matrix's coordinate system to be positive. Because of those two reasons, we switch the subtraction
		// order and subtract camera's target from camera's position. Vector visually ends at the minuend
		// (first operand of subtraction) and starts at the subtrahend (second operand of subtraction). Therefore,
		// we want it to end on camera's position, pointing to it.
		cameraTarget = cameraPosition + cameraFront;
		cameraDirection = glm::normalize(cameraPosition - cameraTarget);

		// Camera's right vector. This vector points right from camera and is perpendicular to "camera's direction".
		// We can create it by doing a cross product between "up" vector and "camera's direction". Result of a
		// cross product is a vector perpendicular to both vectors and we will get a vector that points in the
		// positive x-axis' direction.
		cameraRight = glm::normalize(glm::cross(upVector, cameraDirection));
		// Camera's up vector. Since we have vectors that point in the positive z-axis' direction ("camera's
		// direction") and the positive x-axis's direction (camera's right vector), their cross product will give
		// us the vector pointing in the positive y-axis's direction (camera's up vector).
		cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
	}

	// Utility function for calculating the view matrix.
	glm::mat4 getCalculatedViewMatrix()
	{
		updateCameraCoordinateSystem();

		// GLM's "lookAt" function requires the camera's position, the camera's target and the "up" vector.
		return glm::lookAt(cameraPosition, cameraTarget, upVector);
	}

	// Utility function for processing keyboard input.
	void processKeyboardInput(std::string key, float deltaTime)
	{
		// The camera will move at a constant speed of 2.5 units per second.
		float cameraSpeed = 2.5f * deltaTime;
		// Move camera forward (away from yourself, in negative z-axis' direction) by adding scaled camera's
		// direction to camera's position.
		if (key == "W")
		{
			cameraPosition += cameraFront * cameraSpeed;
		}
		// Move camera backwards (towards yourself, in positive z-axis' direction) by subtracting scaled camera's
		// direction from camera's position.
		else if (key == "S")
		{
			cameraPosition -= cameraFront * cameraSpeed;
		}
		// Move camera to the left (in negative x-axis' direction) by subtracting scaled right vector
		// (cross product of camera's front vector and "up" vector) from camera's position. We have to normalize
		// that right vector, because cross product could return differently sized vectors depending on the value
		// of "cameraFront". Also, we would move slow or fast depending on camera's orientation, instead of at a
		// consistent speed.
		else if (key == "A")
		{
			cameraPosition -= glm::normalize(glm::cross(cameraFront, upVector)) * cameraSpeed;
		}
		// Move camera to the right (in positive x-axis' direction) by adding scaled right vector (cross product of
		// camera's front vector and "up" vector) to camera's position. We have to normalize that right vector,
		// because cross product could return differently sized vectors depending on the value of "cameraFront".
		// Also, we would move slow or fast depending on camera's orientation, instead of at a consistent speed.
		else if (key == "D")
		{
			cameraPosition += glm::normalize(glm::cross(cameraFront, upVector)) * cameraSpeed;
		}
	}

	// Utility function for processing mouse movement, starting from the 2. step of calculating "camera's front"
	// vector. "Camera's front" vector acts as insurance that however we move, camera keeps lookin straight ahead.
	// Math's explained below. In 2. thing we need to manually create LookAt matrix - "camera's direction":
	// glm::vec3 cameraTarget = cameraPosition + cameraFront;
	// glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget) = glm::normalize(-cameraFront);.
	void processMouseMovement(float xoffset, float yoffset)
	{
		// 2. step: add the offset values to the camera's pitch and yaw values.
		// Mouse movement would be too erratic if we didn't scale it by a sensitivity variable.
		const float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		pitch += yoffset;
		yaw += xoffset;

		// 3. step: add constraints to the minimum and maximum pitch values.
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}
		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}

		// 4. and final step: calculate ACTUAL camera's direction vector, the result of subtracting camera's
		// position from camera's target (thus visually ending at camera's target, minuend of subtraction).
		updateCameraCoordinateSystem();
	}

	// Utility function for processing mouse scrolling.
	void processMouseScroll(float yoffset)
	{
		fov -= yoffset;
		if (fov < 1.0f)
		{
			fov = 1.0f;
		}
		if (fov > 45.0f)
		{
			fov = 45.0f;
		}
	}
};
#endif
