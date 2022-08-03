

// Formats:
// var <NAME> <TYPE>
// var <NAME> <TYPE> = <IV>
// var <NAME> = <IV>
// var <NAME1>, <NAME2> = <IV1> <IV2>
// Notes:

var name string
var width uint64 = 10
var email = "unknown@test.com"
var i, idx, compat = 0, 1, true

// Formats:
// <NAME> := <IV>
// <NAME1>, <NAME2> := <IV1> <IV2>
// Notes:
// * 该格式只能用于函数内部的局部变量

{
	enable := true
	idx, once := 1, true

}
