////////////////////////////////////////////////////////////////////////////
//	Module 		: stalker_animation_offsets.hpp
//	Created 	: 19.12.2007
//	Author		: Alexander Dudin
//	Description : Animation offsets class
////////////////////////////////////////////////////////////////////////////

#ifndef STALKER_ANIMATION_OFFSETS_HPP_INCLUDED
#define STALKER_ANIMATION_OFFSETS_HPP_INCLUDED

#include <map>
#include "xrServer_Objects.h"

struct SRotation;

class animation_offsets
{
public:
    typedef std::map<shared_str, SRotation> AnimationOffsets;

private:
    AnimationOffsets m_animation_offsets;

public:
    SRotation const offsets(shared_str const& animation_id) const;
    void load(LPCSTR section);
};

#endif // STALKER_ANIMATION_OFFSETS_HPP_INCLUDED
