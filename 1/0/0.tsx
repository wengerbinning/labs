import React, { useState } from 'react';
import { Card, Form, Input, Select, Button, Space, Table, Tag,
	Modal, Switch, Tabs, Collapse } from 'antd';
import { PlusOutlined, EditOutlined,
	DeleteOutlined, CopyOutlined } from '@ant-design/icons';
import type { ColumnsType } from 'antd/es/table';

const { Option } = Select;
const { TextArea } = Input;
const { TabPane } = Tabs;
const { Panel } = Collapse;

// 类型定义
type SymbolType = 'macro-switch' | 'macro-constant' | 'macro-function' | 'data' | 'type' | 'function';

interface BaseSymbol {
  id: string;
  name: string;
  description?: string;
  references: string[];
  type: SymbolType;
}

interface MacroSwitch extends BaseSymbol {
  type: 'macro-switch';
  defaultValue: boolean;
  possibleValues: boolean[];
}

interface MacroConstant extends BaseSymbol {
  type: 'macro-constant';
  defaultValue: string | number;
  possibleValues: string[];
}

interface MacroFunction extends BaseSymbol {
  type: 'macro-function';
  parameters: Parameter[];
  hasReturnValue: boolean;
  returnType?: string;
}

interface DataSymbol extends BaseSymbol {
  type: 'data';
  dataType: string;
  defaultValue: any;
}

interface TypeSymbol extends BaseSymbol {
  type: 'type';
  members: Member[];
  methods: Method[];
}

interface FunctionSymbol extends BaseSymbol {
  type: 'function';
  parameters: Parameter[];
  returnType: string;
}

type Symbol = MacroSwitch | MacroConstant | MacroFunction | DataSymbol | TypeSymbol | FunctionSymbol;

interface Parameter {
  name: string;
  type: string;
  description?: string;
}

interface Member {
  name: string;
  type: string;
  description?: string;
}

interface Method {
  name: string;
  parameters: Parameter[];
  returnType: string;
  description?: string;
}

// 符号管理页面组件
const SymbolManagement: React.FC = () => {
  const [symbols, setSymbols] = useState<Symbol[]>([]);
  const [isModalVisible, setIsModalVisible] = useState(false);
  const [editingSymbol, setEditingSymbol] = useState<Symbol | null>(null);
  const [selectedType, setSelectedType] = useState<SymbolType>('macro-switch');
  const [form] = Form.useForm();

  // 表格列定义
  const columns: ColumnsType<Symbol> = [
    {
      title: '名称',
      dataIndex: 'name',
      key: 'name',
      sorter: (a, b) => a.name.localeCompare(b.name),
    },
    {
      title: '类型',
      dataIndex: 'type',
      key: 'type',
      render: (type: SymbolType) => {
        const typeMap = {
          'macro-switch': '宏开关',
          'macro-constant': '宏常量',
          'macro-function': '宏函数',
          'data': '数据',
          'type': '类型',
          'function': '函数',
        };
        return <Tag color={getTagColor(type)}>{typeMap[type]}</Tag>;
      },
      filters: [
        { text: '宏开关', value: 'macro-switch' },
        { text: '宏常量', value: 'macro-constant' },
        { text: '宏函数', value: 'macro-function' },
        { text: '数据', value: 'data' },
        { text: '类型', value: 'type' },
        { text: '函数', value: 'function' },
      ],
      onFilter: (value, record) => record.type === value,
    },
    {
      title: '描述',
      dataIndex: 'description',
      key: 'description',
      ellipsis: true,
    },
    {
      title: '引用列表',
      dataIndex: 'references',
      key: 'references',
      render: (refs: string[]) => (
        <>
          {refs && refs.length > 0 ? (
            refs.map(ref => <Tag key={ref}>{ref}</Tag>)
          ) : (
            <span>无引用</span>
          )}
        </>
      ),
    },
    {
      title: '操作',
      key: 'action',
      render: (_, record) => (
        <Space size="middle">
          <Button type="text" icon={<EditOutlined />} onClick={() => handleEdit(record)}>
            编辑
          </Button>
          <Button type="text" danger icon={<DeleteOutlined />} onClick={() => handleDelete(record.id)}>
            删除
          </Button>
          <Button type="text" icon={<CopyOutlined />} onClick={() => handleDuplicate(record)}>
            复制
          </Button>
        </Space>
      ),
    },
  ];

  const getTagColor = (type: SymbolType) => {
    const colorMap: Record<SymbolType, string> = {
      'macro-switch': 'gold',
      'macro-constant': 'orange',
      'macro-function': 'purple',
      'data': 'blue',
      'type': 'green',
      'function': 'cyan',
    };
    return colorMap[type];
  };

  // 处理新增符号
  const handleAdd = () => {
    setEditingSymbol(null);
    setSelectedType('macro-switch');
    form.resetFields();
    setIsModalVisible(true);
  };

  // 处理编辑符号
  const handleEdit = (symbol: Symbol) => {
    setEditingSymbol(symbol);
    setSelectedType(symbol.type);
    form.setFieldsValue(symbol);
    setIsModalVisible(true);
  };

  // 处理删除符号
  const handleDelete = (id: string) => {
    Modal.confirm({
      title: '确认删除',
      content: '确定要删除这个符号吗？',
      onOk: () => {
        setSymbols(symbols.filter(s => s.id !== id));
      },
    });
  };

  // 处理复制符号
  const handleDuplicate = (symbol: Symbol) => {
    const newSymbol = {
      ...symbol,
      id: `${Date.now()}`,
      name: `${symbol.name}_copy`,
    };
    setSymbols([...symbols, newSymbol]);
  };

  // 处理表单提交
  const handleSubmit = (values: any) => {
    const newSymbol = {
      ...values,
      id: editingSymbol?.id || `${Date.now()}`,
      references: values.references?.split(',').map((s: string) => s.trim()) || [],
    };

    if (editingSymbol) {
      setSymbols(symbols.map(s => s.id === editingSymbol.id ? newSymbol : s));
    } else {
      setSymbols([...symbols, newSymbol]);
    }

    setIsModalVisible(false);
    form.resetFields();
  };

  // 渲染动态表单字段
  const renderDynamicFields = () => {
    switch (selectedType) {
      case 'macro-switch':
        return (
          <>
            <Form.Item
              name="defaultValue"
              label="默认值"
              valuePropName="checked"
              rules={[{ required: true }]}
            >
              <Switch />
            </Form.Item>
            <Form.Item
              name="possibleValues"
              label="取值范围"
              initialValue={[true, false]}
            >
              <Select mode="tags" disabled>
                <Option value="true">true</Option>
                <Option value="false">false</Option>
              </Select>
            </Form.Item>
          </>
        );

      case 'macro-constant':
        return (
          <>
            <Form.Item
              name="defaultValue"
              label="默认值"
              rules={[{ required: true }]}
            >
              <Input />
            </Form.Item>
            <Form.Item
              name="possibleValues"
              label="取值范围"
              rules={[{ required: true }]}
            >
              <Select mode="tags" placeholder="输入可能的取值，用逗号分隔">
                {/* 动态添加选项 */}
              </Select>
            </Form.Item>
          </>
        );

      case 'macro-function':
        return (
          <>
            <Collapse>
              <Panel header="参数列表" key="parameters">
                <Form.List name="parameters">
                  {(fields, { add, remove }) => (
                    <>
                      {fields.map(field => (
                        <Space key={field.key} style={{ display: 'flex', marginBottom: 8 }}>
                          <Form.Item
                            {...field}
                            name={[field.name, 'name']}
                            rules={[{ required: true, message: '参数名不能为空' }]}
                          >
                            <Input placeholder="参数名" />
                          </Form.Item>
                          <Form.Item
                            {...field}
                            name={[field.name, 'type']}
                            rules={[{ required: true, message: '类型不能为空' }]}
                          >
                            <Select placeholder="类型" style={{ width: 120 }}>
                              <Option value="int">int</Option>
                              <Option value="float">float</Option>
                              <Option value="string">string</Option>
                              <Option value="bool">bool</Option>
                              <Option value="void">void</Option>
                            </Select>
                          </Form.Item>
                          <Form.Item
                            {...field}
                            name={[field.name, 'description']}
                          >
                            <Input placeholder="描述" />
                          </Form.Item>
                          <Button type="link" danger onClick={() => remove(field.name)}>
                            删除
                          </Button>
                        </Space>
                      ))}
                      <Button type="dashed" onClick={() => add()} block>
                        添加参数
                      </Button>
                    </>
                  )}
                </Form.List>
              </Panel>
            </Collapse>
            <Form.Item
              name="hasReturnValue"
              label="是否有返回值"
              valuePropName="checked"
            >
              <Switch />
            </Form.Item>
            <Form.Item
              noStyle
              shouldUpdate={(prev, curr) => prev.hasReturnValue !== curr.hasReturnValue}
            >
              {({ getFieldValue }) =>
                getFieldValue('hasReturnValue') && (
                  <Form.Item
                    name="returnType"
                    label="返回类型"
                    rules={[{ required: true }]}
                  >
                    <Select>
                      <Option value="int">int</Option>
                      <Option value="float">float</Option>
                      <Option value="string">string</Option>
                      <Option value="bool">bool</Option>
                      <Option value="void">void</Option>
                    </Select>
                  </Form.Item>
                )
              }
            </Form.Item>
          </>
        );

      case 'data':
        return (
          <>
            <Form.Item
              name="dataType"
              label="数据类型"
              rules={[{ required: true }]}
            >
              <Select>
                <Option value="int">整数</Option>
                <Option value="float">浮点数</Option>
                <Option value="string">字符串</Option>
                <Option value="bool">布尔值</Option>
                <Option value="array">数组</Option>
                <Option value="object">对象</Option>
              </Select>
            </Form.Item>
            <Form.Item
              noStyle
              shouldUpdate={(prev, curr) => prev.dataType !== curr.dataType}
            >
              {({ getFieldValue }) => {
                const dataType = getFieldValue('dataType');
                return (
                  <Form.Item
                    name="defaultValue"
                    label="默认值"
                    rules={[{ required: true }]}
                  >
                    {dataType === 'bool' ? (
                      <Select>
                        <Option value={true}>true</Option>
                        <Option value={false}>false</Option>
                      </Select>
                    ) : dataType === 'int' ? (
                      <Input type="number" />
                    ) : dataType === 'float' ? (
                      <Input type="number" step="0.01" />
                    ) : (
                      <Input />
                    )}
                  </Form.Item>
                );
              }}
            </Form.Item>
          </>
        );

      case 'type':
        return (
          <Tabs defaultActiveKey="members">
            <TabPane tab="成员" key="members">
              <Form.List name="members">
                {(fields, { add, remove }) => (
                  <>
                    {fields.map(field => (
                      <Space key={field.key} style={{ display: 'flex', marginBottom: 8 }}>
                        <Form.Item
                          {...field}
                          name={[field.name, 'name']}
                          rules={[{ required: true, message: '成员名不能为空' }]}
                        >
                          <Input placeholder="成员名" />
                        </Form.Item>
                        <Form.Item
                          {...field}
                          name={[field.name, 'type']}
                          rules={[{ required: true, message: '类型不能为空' }]}
                        >
                          <Input placeholder="类型" />
                        </Form.Item>
                        <Form.Item
                          {...field}
                          name={[field.name, 'description']}
                        >
                          <Input placeholder="描述" />
                        </Form.Item>
                        <Button type="link" danger onClick={() => remove(field.name)}>
                          删除
                        </Button>
                      </Space>
                    ))}
                    <Button type="dashed" onClick={() => add()} block>
                      添加成员
                    </Button>
                  </>
                )}
              </Form.List>
            </TabPane>
            <TabPane tab="方法" key="methods">
              <Form.List name="methods">
                {(fields, { add, remove }) => (
                  <>
                    {fields.map(field => (
                      <Card key={field.key} size="small" style={{ marginBottom: 16 }}>
                        <Form.Item
                          {...field}
                          name={[field.name, 'name']}
                          rules={[{ required: true, message: '方法名不能为空' }]}
                        >
                          <Input placeholder="方法名" />
                        </Form.Item>
                        <Form.List name={[field.name, 'parameters']}>
                          {(subFields, { add: addParam, remove: removeParam }) => (
                            <>
                              {subFields.map(subField => (
                                <Space key={subField.key} style={{ display: 'flex', marginBottom: 8 }}>
                                  <Form.Item
                                    {...subField}
                                    name={[subField.name, 'name']}
                                  >
                                    <Input placeholder="参数名" />
                                  </Form.Item>
                                  <Form.Item
                                    {...subField}
                                    name={[subField.name, 'type']}
                                  >
                                    <Input placeholder="类型" />
                                  </Form.Item>
                                  <Button type="link" danger onClick={() => removeParam(subField.name)}>
                                    删除
                                  </Button>
                                </Space>
                              ))}
                              <Button type="dashed" onClick={() => addParam()} block>
                                添加参数
                              </Button>
                            </>
                          )}
                        </Form.List>
                        <Form.Item
                          {...field}
                          name={[field.name, 'returnType']}
                          label="返回类型"
                        >
                          <Input />
                        </Form.Item>
                        <Form.Item
                          {...field}
                          name={[field.name, 'description']}
                          label="描述"
                        >
                          <Input.TextArea />
                        </Form.Item>
                        <Button type="link" danger onClick={() => remove(field.name)}>
                          删除此方法
                        </Button>
                      </Card>
                    ))}
                    <Button type="dashed" onClick={() => add()} block>
                      添加方法
                    </Button>
                  </>
                )}
              </Form.List>
            </TabPane>
          </Tabs>
        );

      case 'function':
        return (
          <>
            <Collapse>
              <Panel header="参数列表" key="parameters">
                <Form.List name="parameters">
                  {(fields, { add, remove }) => (
                    <>
                      {fields.map(field => (
                        <Space key={field.key} style={{ display: 'flex', marginBottom: 8 }}>
                          <Form.Item
                            {...field}
                            name={[field.name, 'name']}
                            rules={[{ required: true, message: '参数名不能为空' }]}
                          >
                            <Input placeholder="参数名" />
                          </Form.Item>
                          <Form.Item
                            {...field}
                            name={[field.name, 'type']}
                            rules={[{ required: true, message: '类型不能为空' }]}
                          >
                            <Select placeholder="类型" style={{ width: 120 }}>
                              <Option value="int">int</Option>
                              <Option value="float">float</Option>
                              <Option value="string">string</Option>
                              <Option value="bool">bool</Option>
                              <Option value="void">void</Option>
                            </Select>
                          </Form.Item>
                          <Form.Item
                            {...field}
                            name={[field.name, 'description']}
                          >
                            <Input placeholder="描述" />
                          </Form.Item>
                          <Button type="link" danger onClick={() => remove(field.name)}>
                            删除
                          </Button>
                        </Space>
                      ))}
                      <Button type="dashed" onClick={() => add()} block>
                        添加参数
                      </Button>
                    </>
                  )}
                </Form.List>
              </Panel>
            </Collapse>
            <Form.Item
              name="returnType"
              label="返回类型"
              rules={[{ required: true }]}
            >
              <Select>
                <Option value="int">int</Option>
                <Option value="float">float</Option>
                <Option value="string">string</Option>
                <Option value="bool">bool</Option>
                <Option value="void">void</Option>
              </Select>
            </Form.Item>
          </>
        );

      default:
        return null;
    }
  };

  return (
    <div style={{ padding: 24 }}>
      <Card
        title="符号管理"
        extra={
          <Button type="primary" icon={<PlusOutlined />} onClick={handleAdd}>
            新增符号
          </Button>
        }
      >
        <Table
          columns={columns}
          dataSource={symbols}
          rowKey="id"
          pagination={{ pageSize: 10 }}
          expandable={{
            expandedRowRender: (record) => (
              <div style={{ margin: 0 }}>
                <p><strong>详细属性：</strong></p>
                <pre style={{ background: '#f5f5f5', padding: 16, borderRadius: 4 }}>
                  {JSON.stringify(record, null, 2)}
                </pre>
              </div>
            ),
          }}
        />
      </Card>

      <Modal
        title={editingSymbol ? '编辑符号' : '新增符号'}
        visible={isModalVisible}
        onCancel={() => setIsModalVisible(false)}
        width={800}
        footer={null}
      >
        <Form
          form={form}
          layout="vertical"
          onFinish={handleSubmit}
          initialValues={{
            type: selectedType,
            references: [],
            hasReturnValue: false,
          }}
        >
          <Form.Item
            name="name"
            label="名称"
            rules={[{ required: true, message: '请输入符号名称' }]}
          >
            <Input />
          </Form.Item>

          <Form.Item
            name="type"
            label="类型"
            rules={[{ required: true }]}
          >
            <Select onChange={(value: SymbolType) => setSelectedType(value)}>
              <Option value="macro-switch">宏开关</Option>
              <Option value="macro-constant">宏常量</Option>
              <Option value="macro-function">宏函数</Option>
              <Option value="data">数据</Option>
              <Option value="type">类型</Option>
              <Option value="function">函数</Option>
            </Select>
          </Form.Item>

          <Form.Item
            name="description"
            label="描述"
          >
            <TextArea rows={2} />
          </Form.Item>

          <Form.Item
            name="references"
            label="引用列表"
            help="多个引用请用逗号分隔"
          >
            <Input placeholder="例如: file1.c, file2.h, main.c" />
          </Form.Item>

          {renderDynamicFields()}

          <Form.Item style={{ marginBottom: 0, textAlign: 'right' }}>
            <Space>
              <Button onClick={() => setIsModalVisible(false)}>
                取消
              </Button>
              <Button type="primary" htmlType="submit">
                确定
              </Button>
            </Space>
          </Form.Item>
        </Form>
      </Modal>
    </div>
  );
};

export default SymbolManagement;
