/*
 This program is free s*oftware; you can redistribute it and/or
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

#ifndef _func_ptr_h_
#define _func_ptr_h_

#include <functional>
#include <stdexcept>
#include <memory>

namespace edward
{

class copied_unique_ptr_error: public std::logic_error
{
public:
	copied_unique_ptr_error(): std::logic_error("A unique pointer within edward::func_ptr was attempted to be copied") {}
};

template<typename T>
class func_ptr
{
private:
	enum command_t {GET, COPY, DELETE};
	
public:
	using ptr_t = T*;
	//TODO: Make this accept a boolena arg which tells the object to release
	using func_t = std::function<ptr_t(command_t)>;

private:
	func_t fn_;
	
private:
	class unique_ptr_wrap: public std::unique_ptr<T>
	{
	
	public:
		using std::unique_ptr<T>::unique_ptr;
		unique_ptr_wrap(std::unique_ptr<T>&& other): std::unique_ptr<T>(std::move(other)) {}
		unique_ptr_wrap(const unique_ptr_wrap& other): std::unique_ptr<T>(other.get())
		{
		}
		
		virtual ~unique_ptr_wrap()
		{
			std::unique_ptr<T>::release();
		}
		
	public:
		ptr_t operator()(command_t command)
		{
			switch(command)
			{
			case COPY:
				throw copied_unique_ptr_error();
				return nullptr;
			case DELETE:
				std::unique_ptr<T>::reset(nullptr);
				return nullptr;
			case GET:
				return std::unique_ptr<T>::get();
			}
		}
	};
	
	class internal_object_wrap
	{
	public:
		internal_object_wrap(T init): obj_(std::move(init)) {}
		
	public:
		ptr_t operator()(command_t command) const {return &obj_;}
		
	private:
		mutable T obj_;
	};
	
public:
	/// default constructor
	func_ptr() = default;
	
	/// copy constructor
	func_ptr(const func_ptr<T>& other): fn_(other.fn_)
	{
		if(fn_)
			fn_(COPY);
	}
	
	/// copy assign
	func_ptr<T>& operator= (const func_ptr<T>&) = default;
	
	/// move constructor
	func_ptr(func_ptr<T>&&) = default;
	
	/// move assign
	func_ptr<T>& operator= (func_ptr<T>&&) = default;
	
	/// destructor
	~func_ptr()
	{
		if(fn_)
			fn_(DELETE);
	}
	
	/// Generic pointer returning function
	func_ptr(func_t fn): fn_(fn) {}
	
	/// Wraps raw pointer which it won't destroy
	func_ptr(ptr_t ptr): fn_([ptr](command_t command) {return ptr;}) {}
	
	/// Wraps a shared_ptr which will destroy when the last reference to it does
	func_ptr(std::shared_ptr<T> ptr): fn_([ptr](command_t command) {return ptr.get();}) {}
	
	/// Wraps a unique_ptr by putting it in a specially defined callable object which throws when its copied
	func_ptr(std::unique_ptr<T> ptr): fn_(unique_ptr_wrap(std::move(ptr))) {}
	
	/// Wraps the object in place
	func_ptr(T obj): fn_(internal_object_wrap(std::move(obj))) {}
	
public:
	/// Getter
	ptr_t get() const {if(fn_) return fn_(GET); else return nullptr;}
	
	/// Implicit conversion
	operator ptr_t() const {if(fn_) return get(); else return nullptr;}
	
	/// Dereference member access
	ptr_t operator-> () const {if(fn_) return get(); else return nullptr;}
	
	/// Dereference
	T& operator* () const {return *get();}
};

}

#endif // _func_ptr_h_