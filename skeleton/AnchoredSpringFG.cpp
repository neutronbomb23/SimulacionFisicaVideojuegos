#include "AnchoredSpringFG.h"

AnchoredSpringFG::AnchoredSpringFG(double k, double resting, const Vector3& anchor_pos) :
SpringForceGenerator(k, resting, nullptr)
{
	
	PxTransform tr;
	tr.p = anchor_pos;
	Vector4 color = Vector4(0, 1, 0, 1);
	Vector3 gS = Vector3(0, -9.8, 0);
	PxShape* s = CreateShape(PxBoxGeometry(1, 1, 1));
	_other = new Particle(s, tr, { 0, 0, 0 }, { 0, 0, 0 }, gS, 1e6, color);
	RegisterRenderItem(_other->getRenderItem());
}
