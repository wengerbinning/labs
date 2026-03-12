

export type ModelType = 'type' | 'function' | 'interface' | 'event' |
	'object' | 'task';


export interface BaseModel {
	id: string;
	name: string;
	desc: string;
	type: ModelType;
	time: string[];

	icons: string[];
	attrs: string[];
	state: string[];
	label: string[];

	version: string[];
	imports: string[];
	exports: string[];
}

export interface TypeModel extends BaseModel {
	method: string[];
}

export interface FunctionModel extends BaseModel {
	method: string[];
}

export interface EventModel extends BaseModel {
	method: string[];
}

export interface ObjectModel extends BaseModel {
	method: string[];
}

export interface TaskModel extends BaseModel {
	event: string[];
}
