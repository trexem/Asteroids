#pragma once
#ifndef __RenderComponent_H_
#define __RenderComponent_H_

#include "Component.h"

class RenderComponent : public Component {
public:
	virtual ~RenderComponent() = default;

	virtual void Render() = 0;
};
#endif // !__RenderComponent_H_
