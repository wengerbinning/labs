
export type SymbolType = 'macro-switch' | 'macro-constant' | 'macro-function' | 'data' | 'type' | 'function';


export interface Parameter {
  name: string;
  type: string;
  description?: string;
}

export interface Member {
  name: string;
  type: string;
  description?: string;
}

export interface Method {
  name: string;
  parameters: Parameter[];
  returnType: string;
  description?: string;
}

export interface BaseSymbol {
  uuid  : string;
  name  : string;
  desc ?: string;
  type ?: SymbolType;
}

export interface MacroSwitch extends BaseSymbol {
  type  : 'macro-switch';
  defaultValue ?: boolean;
  possibleValues ?: boolean[];
}

export interface MacroConstant extends BaseSymbol {
  type: 'macro-constant';
  defaultValue: string | number;
  possibleValues: string[];
}

export interface MacroFunction extends BaseSymbol {
  type: 'macro-function';
  parameters: Parameter[];
  hasReturnValue: boolean;
  returnType?: string;
}

export interface DataSymbol extends BaseSymbol {
  type: 'data';
  dataType: string;
  defaultValue: any;
}

export interface TypeSymbol extends BaseSymbol {
  type: 'type';
  members: Member[];
  methods: Method[];
}

export interface FunctionSymbol extends BaseSymbol {
  type: 'function';
  parameters: Parameter[];
  returnType: string;
}

export type Symbol = BaseSymbol |
	MacroSwitch | MacroConstant | MacroFunction |
	DataSymbol | TypeSymbol | FunctionSymbol;
