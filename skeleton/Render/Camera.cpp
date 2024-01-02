// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2018 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  



#include "Camera.h"
#include <ctype.h>
#include "foundation/PxMat33.h"
#include "../Generator.h"
#include "../Matrix4x4.h"


using namespace physx;

namespace Snippets
{

Camera::Camera(const PxVec3& eye, const PxVec3& dir)
{
	mEye = eye;
	mDir = dir.getNormalized();
	mMouseX = 0;
	mMouseY = 0;
}

void Camera::handleMouse(int button, int state, int x, int y)
{
	/*PX_UNUSED(state);
	PX_UNUSED(button);
	mMouseX = x;
	mMouseY = y;*/
	//if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	//{
	//	// Suponiendo que tienes acceso a las matrices de proyección y vista de la cámara
	//	Matrix4x4 projectionMatrix; // Matriz de proyección de la cámara
	//	Matrix4x4 viewMatrix; // Matriz de vista de la cámara

	//	// Obtener las coordenadas de pantalla al hacer clic
	//	int x_screen = x;
	//	int y_screen = y;

	//	// Convertir las coordenadas de pantalla a coordenadas de clip (entre -1 y 1)
	//	float x_clip = ((2.0f * x_screen) / 1920) - 1.0f;
	//	float y_clip = 1.0f - ((2.0f * y_screen) / 1080);

	//	// Convertir las coordenadas de clip a coordenadas de vista (sin aplicar perspectiva)
	//	Vector4 clipCoords(x_clip, y_clip, -1.0f, 1.0f);
	//	Vector4 eyeCoords = projectionMatrix.inverse() * clipCoords;
	//	eyeCoords.z = -1.0f; // Punto en el espacio cercano a la cámara
	//	eyeCoords.w = 0.0f;

	//	// Convertir las coordenadas de vista a coordenadas del mundo 3D
	//	Vector4 worldCoords = viewMatrix.inverse() * eyeCoords;
	//	Vector3 mouseWorldPosition(worldCoords.x, worldCoords.y, worldCoords.z);

	//	// Aquí `mouseWorldPosition` contiene las coordenadas en el espacio 3D
	//	// Puedes usar estas coordenadas según sea necesario
	//	Gen->shoot(mouseWorldPosition);
	//}
}

bool Camera::handleKey(unsigned char key, int x, int y, float speed)
{
	PX_UNUSED(x);
	PX_UNUSED(y);

	PxVec3 viewY = mDir.cross(PxVec3(0,1,0)).getNormalized();
	const float limit = 90.0f;
	switch(toupper(key))
	{
	case 'W':	mEye += mDir*2.0f*speed;		break;
	case 'S':	mEye -= mDir*2.0f*speed;		break;
	case 'A':	
		mEye -= viewY*2.0f*speed;	 
		if (mEye.magnitude() > limit) {
			mEye += viewY * 2.0f * speed; // Revierte el movimiento si excede el límite
		}
		break;
	case 'D':	
		mEye += viewY * 2.0f * speed;		
		if (mEye.magnitude() > limit) {
			mEye -= viewY * 2.0f * speed; // Revierte el movimiento si excede el límite
		}
		break;
	default: return false;
	}
	return true;
}

void Camera::handleAnalogMove(float x, float y)
{
	PxVec3 viewY = mDir.cross(PxVec3(0,1,0)).getNormalized();
	mEye += mDir*y;
	mEye += viewY*x;
}

void Camera::handleMotion(int x, int y)
{
	/*int dx = mMouseX - x;
	int dy = mMouseY - y;

	PxVec3 viewY = mDir.cross(PxVec3(0,1,0)).getNormalized();

	PxQuat qx(PxPi * dx / 180.0f, PxVec3(0,1,0));
	mDir = qx.rotate(mDir);
	PxQuat qy(PxPi * dy / 180.0f, viewY);
	mDir = qy.rotate(mDir);

	mDir.normalize();

	mMouseX = x;
	mMouseY = y;*/
}

PxTransform Camera::getTransform() const
{
	PxVec3 viewY = mDir.cross(PxVec3(0,1,0));

	if(viewY.normalize()<1e-6f) 
		return PxTransform(mEye);

	PxMat33 m(mDir.cross(viewY), viewY, -mDir);
	return PxTransform(mEye, PxQuat(m));
}

PxVec3 Camera::getEye() const
{ 
	return mEye; 
}

PxVec3 Camera::getDir() const
{ 
	return mDir; 
}


}

