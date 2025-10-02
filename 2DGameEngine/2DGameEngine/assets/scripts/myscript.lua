-- this is a Lua global variable
some_variable = 3 + 2

-- This is a Lua table that contain values for the initialization of the game engine
config = {
	title = "STAGE: Simple Two-dimensional Animation Game Engine",
	fullscreen = false,
	resolution = {
		width = 800,
		height = 600
	}
}

-- This is a Lua function that calculates the factorial of a given number
function factorial(n)
if n == 1 then
	return 1
end
return n * factorial(n - 1)
end

print("Factorial of 5 is "..factorial(5))