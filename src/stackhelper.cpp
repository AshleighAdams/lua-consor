#ifndef STACKHELPER_HPP
#define STACKHELPER_HPP

#include <lua.hpp>
#include <string>
#include <exception>

void lua_check(lua_State* L, bool condition, const std::string& message)
{
	if(condition)
		return;
	lua_pushstring(L, message.c_str());
	lua_error(L);
}

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
int object_fromptr(void* ptr, int type)
{
	for(auto pair : objects)
		if(pair.second.ptr == ptr && pair.second.type == type)
			return pair.first;
	return 0;
}

template <typename T>
struct ObjectTypes;

template <> struct ObjectTypes<IConsoleRenderer>          {static const int Type {1};};
template <> struct ObjectTypes<PlatformConsoleRenderer>   {static const int Type {1};};

template <> struct ObjectTypes<IInputSystem>              {static const int Type {2};};
template <> struct ObjectTypes<PlatformInputSystem>       {static const int Type {2};};

// Skins
template <> struct ObjectTypes<ISkin>                     {static const int Type {4};};

// All controls need to be under this type :C, in future, ill add this up so controls can't be passed between eachother.
template <> struct ObjectTypes<Control>                   {static const int Type {5};};

// Containers
template <> struct ObjectTypes<AlignContainer>            {static const int Type {5};};
template <> struct ObjectTypes<BorderContainer>           {static const int Type {5};};
template <> struct ObjectTypes<FlowContainer>             {static const int Type {5};};
template <> struct ObjectTypes<ScrollContainer>           {static const int Type {5};};
template <> struct ObjectTypes<WindowContainer>           {static const int Type {5};};

// Controls
template <> struct ObjectTypes<Button>                    {static const int Type {5};};
template <> struct ObjectTypes<CheckBox>                  {static const int Type {5};};
template <> struct ObjectTypes<Graph>                     {static const int Type {5};};
template <> struct ObjectTypes<HorizontalScrollbar>       {static const int Type {5};};
template <> struct ObjectTypes<Label>                     {static const int Type {5};};
template <> struct ObjectTypes<PasswordBox>               {static const int Type {5};};
template <> struct ObjectTypes<ProgressBar>               {static const int Type {5};};
template <> struct ObjectTypes<RadioBox>                  {static const int Type {5};};
template <> struct ObjectTypes<TextBox>                   {static const int Type {5};};
template <> struct ObjectTypes<VerticalProgressBar>       {static const int Type {5};};
template <> struct ObjectTypes<VerticalScrollbar>         {static const int Type {5};};



// Containers

template <typename T>
struct Object;

template <typename T>
struct Object//<T>
{
	template <typename... Args>
	static int Make(Args&&... args) // GOT IT, THIS IS BEING DISPOSED, AND THUS ~T() IS BEING CALLED!
	{
		T* ptr = new T(args...);
		return make_object(ptr, ObjectTypes<T>::Type);
	}
	static void Destroy(int Id)
	{
		T* ptr = Get(Id);
		if(ptr)
		{
			remove_object(Id);
			delete ptr;
		}
	}
	static T* Get(int Id)
	{
		object_t* obj = get_object(Id, ObjectTypes<T>::Type);
		return obj ? static_cast<T*>(obj->ptr) : nullptr;
	}
	static int FromPointer(T* Ptr)
	{
		return object_fromptr(Ptr, ObjectTypes<T>::Type);
	}
};

/// LUA STUFF

template <typename T>
struct Stack;

// void / nil

template <>
struct Stack<void>
{
	static void Get(lua_State* L, int Index)
	{
	}
	
	static bool Check(lua_State* L, int Index)
	{
		return lua_isnoneornil(L, Index) == 1;
	}
	
	static void Push(lua_State* L, int Value)
	{
		lua_pushnil(L);
	}
};

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
struct Stack<size_t>
{
	static size_t Get(lua_State* L, int Index)
	{
		return lua_tointeger(L, Index);
	}
	
	static bool Check(lua_State* L, int Index)
	{
		return lua_isnumber(L, Index) == 1;
	}
	
	static void Push(lua_State* L, size_t Value)
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


// Convert a function on the stack at index position to a std::function<void(...)>
struct lualock
{
	lua_State* L;
	lualock(lua_State* l) : L(l)
	{
		//lua_lock(L);
	}
	~lualock()
	{
		//lua_unlock(L);
	}
};

// This is to be held with a shared ptr
struct lua_function_reference_backend
{
	lua_State* L;
	int ref;
	
	lua_function_reference_backend(lua_State* l, int index) : L(l)
	{
		lualock lock(L);
		lua_pushvalue(L, index);
		ref = luaL_ref(L, LUA_REGISTRYINDEX);
	}
	
	~lua_function_reference_backend()
	{
		try
		{
			lualock lock(L);
			// FIXME: why would this fail?
			luaL_unref(L, LUA_REGISTRYINDEX, ref);
		}catch(...)
		{
			// The lua state must've already been destroyed?
		}
	}
};


// Allow a return value
template <typename T> struct lua_function_reference;

template<typename R, typename... Args>
struct lua_function_reference <R(Args...)>
{
	std::shared_ptr<lua_function_reference_backend> ref;
	
	lua_function_reference(lua_State* L, int Index) :
		ref(std::make_shared<lua_function_reference_backend>(L, Index))
	{}
	
	// This is called at the end
	void PushArgs(int& nargs) {}
	
	template<typename T, typename... Args2>
	void PushArgs(int& nargs, T first, Args2... args)
	{
		Stack<T>::Push(ref->L, first);
		nargs++;
		this->PushArgs(nargs, args...);
	}
	
	R operator()(Args... args)
	{
		lualock lock(ref->L);
		lua_rawgeti(ref->L, LUA_REGISTRYINDEX, ref->ref);
		
		int nargs = 0;
		int nrets = 1;
		this->PushArgs(nargs, args...);
		lua_call(ref->L, nargs, nrets);
		
		return Stack<R>::Get(ref->L, -1);
	}
	
};


#endif


