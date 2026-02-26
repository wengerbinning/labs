

const symbols = [{
	"name": "func0",
	"type": "function",
	"desc": "",
	"attr": [],
	"data": "",
	"label": [],

	/* function */
	"return": {
		"type": "int",
		"desc": ""
	},
	"params": [{
		"name": "a",
		"type": "int",
		"desc": ""
	}, {
		"name": "b",
		"type": "int",
		"desc": ""
	}]
}, {
	"name": "t0_t",
	"type": "type",
	"desc": ""
}]


function symb_prototype(symb) {
	let buf = '';
	switch(symb.type) {
		case "function":
			buf += symb.return.type + ' '+ symb.name + ' (';
			for (let i = 0; i < symb.params.length; i++) {
				buf += symb.params[i].type + ' ' + symb.params[i].name;
				if (i + 1 != symb.params.length)
					buf += ', ';
			}
			buf += ');';
			break;
		case "type":
			buf += symb.name + ';';
			break;
		default:
			console.log("warning: unknown " + symb.type);
	}
	return buf;
}
function symb_implement(symb){
	let buf = '';
	return buf;
}
function symb_attribute(symb){

}



for (let i = 0; i < symbols.length; i++) {
	console.log(symb_prototype(symbols[i]));
}
