#include "AnchoredSpringFG.h"

AnchoredSpringFG::AnchoredSpringFG(double k, double resting, const Vector3& anchor_pos) :
SpringForceGenerator(k, resting, nullptr)
{
	Vector4 color = Vector4(0, 1, 0, 1);
	Vector3 gS = Vector3(0, -9.8, 0) /** powf((velS / velR), 2)*/;// gravedad simulada
	PxShape* s = CreateShape(PxBoxGeometry(1, 1, 1));
	PxTransform tr;
	tr.p = anchor_pos;
	//_other = new Particle(tr, { 0, 0, 0 }, { 0, 0, 0 }, 0, 1e6, 0.0, BOX);
	_other = new Particle(s, tr, { 0, 0, 0 }, { 0, 0, 0 }, gS, 1e6, color);
	RegisterRenderItem(_other->getRenderItem());
}
