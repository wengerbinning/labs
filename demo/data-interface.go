// 接口
// Format: type <NAME> interface {}
// Features:
// * 隐式实现，只要类型实现所有接口方法，即自动满足接口
// Notes:


type LogOps interface {
	Debug() string
	Note() string
	Notice() string
	Warning() string
	Error() string
	Painc() string
}

type Device struct {
	name string
}

func (dev Device) Debug() string {
	return dev.name;
}
