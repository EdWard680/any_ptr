/*
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA  02110-1301, USA.

 ---
Copyright (C) 2016, Eric Ward <ezeward4@gmail.com>
*/

#ifndef _any_ptr_h_
#define _any_ptr_h_

#include <memory>
#include <functional>

namespace edward
{

template<typename T>
class any_ptr
{
public:
	any_ptr();
	~any_ptr();
	
private:
	union pointer_types
	{
		T *raw_;
		std::unique_ptr<T> unique_;
		std::shared_ptr<T> shared_;
	}
	
};

}

#endif // _any_ptr_h_