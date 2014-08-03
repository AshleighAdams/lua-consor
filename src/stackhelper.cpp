#ifndef STACKHELPER_HPP
#define STACKHELPER_HPP

#include <lua.hpp>
#include <string>
#include <exception>

struct object_t
{
	void* ptr;
	int type;
	int id;
};

int object_current_id = 1;
unordered_map<int, object_t> objects;
object_t* get_object(int id, int type)
{
	auto search = objects.find(id);
	if(search == objects.end())
		return nullptr;
	
	if(type != 0)
		return search->second.type != type ? nullptr : &search->second;
	else
		return &search->second;
}

int make_object(void* ptr, int type)
{
	int id = object_current_id++;
	object_t obj {ptr, type, id};
	objects[id] = obj;
	return id;
}
void remove_object(int id)
{
	objects.erase(id);
}
int object_fromptr(void* ptr)
{
	for(auto pair : objects)
		if(pair.second.ptr == ptr)
			return pair.first;
	return 0;
}

template <typename T>
struct ObjectTypes;

template <> struct ObjectTypes<IConsoleRenderer>          {static const int Type {1};};
template <> struct ObjectTypes<PlatformConsoleRenderer>   {static const int Type {1};};

template <typename T>
struct Object;

template <typename T>
struct Object//<T>
{
	template <typename... Args>
	static int Make(Args... args)
	{
		T* ptr = new T(args...);
		std::cout << "making " << ptr << "\n";
		return make_object(ptr, ObjectTypes<T>::Type);
	}
	static void Destroy(int Id)
	{
		T* ptr = Get(Id);
		std::cout << "deleting " << ptr << "\n";
		if(ptr)
		{
			remove_object(Id);
			delete ptr;
		}
	}
	static T* Get(int Id)
	{
		return static_cast<T*>(get_object(Id, ObjectTypes<T>::Type)->ptr);
	}
};

/// LUA STUFF

template <typename T>
struct Stack;

// Numbers

template <>
struct Stack<int>
{
	static int Get(lua_State* L, int Index)
	{
		return lua_tointeger(L, Index);
	}
	
	static bool Check(lua_State* L, int Index)
	{
		return lua_isnumber(L, Index) == 1;
	}
	
	static void Push(lua_State* L, int Value)
	{
		lua_pushinteger(L, Value);
	}
};

template <>
struct Stack<double>
{
	static double Get(lua_State* L, int Index)
	{
		return lua_tonumber(L, Index);
	}
	
	static bool Check(lua_State* L, int Index)
	{
		return lua_isnumber(L, Index) == 1;
	}
	
	static void Push(lua_State* L, double Value)
	{
		lua_pushnumber(L, Value);
	}
};

// Functions

template <>
struct Stack<lua_CFunction>
{
	static lua_CFunction Get(lua_State* L, int Index)
	{
		throw std::runtime_error("can't get c function!");
		return nullptr;
	}
	
	static bool Check(lua_State* L, int Index)
	{
		return lua_isfunction(L, Index) == 1;
	}
	
	static void Push(lua_State* L, lua_CFunction Value)
	{
		lua_pushcfunction(L, Value);
	}
};

// Strings

template <>
struct Stack<std::string>
{
	static std::string Get(lua_State* L, int Index)
	{
		return lua_tostring(L, Index);
	}
	
	static bool Check(lua_State* L, int Index)
	{
		return lua_isstring(L, Index) == 1;
	}
	
	static void Push(lua_State* L, const std::string& Value)
	{
		lua_pushstring(L, Value.c_str());
	}
};


// Boolean

template <>
struct Stack<bool>
{
	static bool Get(lua_State* L, int Index)
	{
		return lua_toboolean(L, Index) == 1;
	}
	
	static bool Check(lua_State* L, int Index)
	{
		return lua_isboolean(L, Index) == 1;
	}
	
	static void Push(lua_State* L, bool Value)
	{
		lua_pushboolean(L, Value);
	}
};

// Consor Units

template <>
struct Stack<Size>
{
	static Size Get(lua_State* L, int Index)
	{
		if(!lua_istable(L, Index))
			return Size();
		
		lua_getfield(L, Index, "Width");
		lua_getfield(L, Index, "Height");
		
		double width = lua_tonumber(L, -2);
		double height = lua_tonumber(L, -1);
		
		lua_pop(L, 2);
		return Size(width, height);
	}
	
	static bool Check(lua_State* L, int Index)
	{
		if(!lua_istable(L, Index))
			return false;
		
		lua_getfield(L, Index, "Width");
		lua_getfield(L, Index, "Height");
		
		bool valid = lua_isnumber(L, -2) && lua_isnumber(L, -1);
		lua_pop(L, 2);
		return valid;
	}
	
	static void Push(lua_State* L, const Size& Value)
	{
		lua_newtable(L);
		lua_pushnumber(L, Value.Width);
		lua_setfield(L, -2, "Width");
		lua_pushnumber(L, Value.Height);
		lua_setfield(L, -2, "Height");
	}
};

template <>
struct Stack<Vector>
{
	static Vector Get(lua_State* L, int Index)
	{
		if(!lua_istable(L, Index))
			return Vector();
		
		lua_getfield(L, Index, "X");
		lua_getfield(L, Index, "Y");
		
		double x = lua_tonumber(L, -2);
		double y = lua_tonumber(L, -1);
		
		lua_pop(L, 2);
		return Vector(x, y);
	}
	
	static bool Check(lua_State* L, int Index)
	{
		if(!lua_istable(L, Index))
			return false;
		
		lua_getfield(L, Index, "X");
		lua_getfield(L, Index, "Y");
		
		bool valid = lua_isnumber(L, -2) && lua_isnumber(L, -1);
		lua_pop(L, 2);
		return valid;
	}
	
	static void Push(lua_State* L, const Vector& Value)
	{
		lua_newtable(L);
		lua_pushnumber(L, Value.X);
		lua_setfield(L, -2, "X");
		lua_pushnumber(L, Value.Y);
		lua_setfield(L, -2, "Y");
	}
};


template <>
struct Stack<Colour>
{
	static Colour Get(lua_State* L, int Index)
	{
		if(!lua_istable(L, Index))
			return Colour();
		
		lua_getfield(L, Index, "R");
		lua_getfield(L, Index, "G");
		lua_getfield(L, Index, "B");
		lua_getfield(L, Index, "A");
		
		double r = lua_tonumber(L, -4);
		double g = lua_tonumber(L, -3);
		double b = lua_tonumber(L, -2);
		double a = lua_tonumber(L, -1);
		
		lua_pop(L, 4);
		return Colour(r, g, b, a);
	}
	
	static bool Check(lua_State* L, int Index)
	{
		if(!lua_istable(L, Index))
			return false;
		
		lua_getfield(L, Index, "R");
		lua_getfield(L, Index, "G");
		lua_getfield(L, Index, "B");
		lua_getfield(L, Index, "A");
		
		bool valid = lua_isnumber(L, -4) &&
		             lua_isnumber(L, -3) &&
		             lua_isnumber(L, -2) &&
		             lua_isnumber(L, -1);
		lua_pop(L, 4);
		return valid;
	}
	
	static void Push(lua_State* L, const Colour& Value)
	{
		lua_newtable(L);
		lua_pushnumber(L, Value.R);
		lua_setfield(L, -2, "R");
		lua_pushnumber(L, Value.G);
		lua_setfield(L, -2, "G");
		lua_pushnumber(L, Value.B);
		lua_setfield(L, -2, "B");
		lua_pushnumber(L, Value.A);
		lua_setfield(L, -2, "A");
	}
};
#endif
