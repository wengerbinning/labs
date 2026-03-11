import React, { useState } from 'react';
import {
  Card, Form, Input, Select, Button, Space, Table, Tag,
  Modal, Switch, Tabs, Collapse, message, Tooltip } from 'antd';
import {
  PlusOutlined, EditOutlined, DeleteOutlined, CopyOutlined,
  ExportOutlined, ImportOutlined, SearchOutlined } from '@ant-design/icons';
import type { ColumnsType } from 'antd/es/table';
import {
  Symbol, SymbolType, MacroSwitch, MacroConstant, MacroFunction,
  DataSymbol, TypeSymbol, FunctionSymbol } from '../types/symbol';

const { Option } = Select;
const { TextArea } = Input;
const { TabPane } = Tabs;
const { Panel } = Collapse;

const DisplayPanel: React.FC = () => {
  const [symbols, setSymbols] = useState<Symbol[]>([]);
  const [isModalVisible, setIsModalVisible] = useState(false);
  const [editingSymbol, setEditingSymbol] = useState<Symbol | null>(null);
  const [selectedType, setSelectedType] = useState<SymbolType>('macro-switch');
  const [searchText, setSearchText] = useState('');
  const [form] = Form.useForm();

  // 获取标签颜色
  const getTagColor = (type: SymbolType): string => {
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

  // 获取类型中文名
  const getTypeName = (type: SymbolType): string => {
    const typeMap: Record<SymbolType, string> = {
      'macro-switch': '宏开关',
      'macro-constant': '宏常量',
      'macro-function': '宏函数',
      'data': '数据',
      'type': '类型',
      'function': '函数',
    };
    return typeMap[type];
  };

  // 表格列定义
// 表格列定义
const columns: ColumnsType<Symbol> = [
	{
	  title: '名称',
	  dataIndex: 'name',
	  key: 'name',
	  sorter: (a: Symbol, b: Symbol) => a.name.localeCompare(b.name),
	  filteredValue: searchText ? [searchText] : null,
	//   onFilter: (value: string | number | boolean, record: Symbol) => {
	// 	// 修复类型错误：确保 value 是字符串
	// 	if (typeof value === 'string') {
	// 	  return record.name.toLowerCase().includes(value.toLowerCase());
	// 	}
	// 	return false;
	//   },
	},
	{
	  title: '类型',
	  dataIndex: 'type',
	  key: 'type',
	  render: (type: SymbolType) => (
		<Tag color={getTagColor(type)}>{getTypeName(type)}</Tag>
	  ),
	  filters: [
		{ text: '宏开关', value: 'macro-switch' },
		{ text: '宏常量', value: 'macro-constant' },
		{ text: '宏函数', value: 'macro-function' },
		{ text: '数据', value: 'data' },
		{ text: '类型', value: 'type' },
		{ text: '函数', value: 'function' },
	  ],
	//   onFilter: (value: string | number | boolean, record: Symbol) => {
	// 	// 修复类型错误
	// 	return record.type === value;
	//   },
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
			<span style={{ color: '#999' }}>无引用</span>
		  )}
		</>
	  ),
	},
	{
	  title: '操作',
	  key: 'action',
	  width: 200,
	  render: (_: any, record: Symbol) => (
		<Space size="middle">
		  <Tooltip title="编辑">
			<Button type="text" icon={<EditOutlined />} onClick={() => handleEdit(record)} />
		  </Tooltip>
		  <Tooltip title="复制">
			<Button type="text" icon={<CopyOutlined />} onClick={() => handleDuplicate(record)} />
		  </Tooltip>
		  <Tooltip title="删除">
			<Button type="text" danger icon={<DeleteOutlined />} onClick={() => handleDelete(record.id)} />
		  </Tooltip>
		</Space>
	  ),
	},
  ];

  // 处理新增符号
  const handleAdd = () => {
    setEditingSymbol(null);
    setSelectedType('macro-switch');
    form.resetFields();
    form.setFieldsValue({
      type: 'macro-switch',
      references: [],
      hasReturnValue: false,
    });
    setIsModalVisible(true);
  };

  // 处理编辑符号
  const handleEdit = (symbol: Symbol) => {
    setEditingSymbol(symbol);
    setSelectedType(symbol.type);

    // 格式化表单数据
    const formData = {
      ...symbol,
      references: symbol.references?.join(', '),
    };

    form.setFieldsValue(formData);
    setIsModalVisible(true);
  };

  // 处理删除符号
  const handleDelete = (id: string) => {
    Modal.confirm({
      title: '确认删除',
      content: '确定要删除这个符号吗？此操作不可恢复。',
      okText: '确认',
      cancelText: '取消',
      onOk: () => {
        setSymbols(symbols.filter(s => s.id !== id));
        message.success('删除成功');
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
    message.success('复制成功');
  };

  // 处理表单提交
  const handleSubmit = (values: any) => {
    try {
      // 处理引用列表
      const references = values.references
        ? values.references.split(',').map((s: string) => s.trim()).filter((s: string) => s)
        : [];

      const newSymbol: Symbol = {
        ...values,
        id: editingSymbol?.id || `${Date.now()}`,
        references,
      };

      if (editingSymbol) {
        setSymbols(symbols.map(s => s.id === editingSymbol.id ? newSymbol : s));
        message.success('更新成功');
      } else {
        setSymbols([...symbols, newSymbol]);
        message.success('添加成功');
      }

      setIsModalVisible(false);
      form.resetFields();
    } catch (error) {
      message.error('操作失败');
    }
  };

  // 处理导入
  const handleImport = () => {
    const input = document.createElement('input');
    input.type = 'file';
    input.accept = '.json';
    input.onchange = (e: any) => {
      const file = e.target.files[0];
      const reader = new FileReader();
      reader.onload = (e) => {
        try {
          const data = JSON.parse(e.target?.result as string);
          if (Array.isArray(data)) {
            setSymbols(data);
            message.success('导入成功');
          } else {
            message.error('文件格式错误');
          }
        } catch (error) {
          message.error('导入失败');
        }
      };
      reader.readAsText(file);
    };
    input.click();
  };

  // 处理导出
  const handleExport = () => {
    const dataStr = JSON.stringify(symbols, null, 2);
    const dataUri = 'data:application/json;charset=utf-8,'+ encodeURIComponent(dataStr);
    const exportFileDefaultName = `symbols_${new Date().getTime()}.json`;

    const linkElement = document.createElement('a');
    linkElement.setAttribute('href', dataUri);
    linkElement.setAttribute('download', exportFileDefaultName);
    linkElement.click();
    message.success('导出成功');
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
              rules={[{ required: true, message: '请选择默认值' }]}
            >
              <Switch checkedChildren="开启" unCheckedChildren="关闭" />
            </Form.Item>
            <Form.Item
              name="possibleValues"
              label="取值范围"
              initialValue={[true, false]}
            >
              <Select mode="multiple" disabled>
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
              rules={[{ required: true, message: '请输入默认值' }]}
            >
              <Input placeholder="请输入默认值" />
            </Form.Item>
            <Form.Item
              name="possibleValues"
              label="取值范围"
              rules={[{ required: true, message: '请输入取值范围' }]}
            >
              <Select mode="tags" placeholder="输入可能的取值，回车确认">
                {/* 动态添加选项 */}
              </Select>
            </Form.Item>
          </>
        );

      case 'macro-function':
        return (
          <>
            <Collapse defaultActiveKey={['parameters']}>
              <Panel header="参数列表" key="parameters">
                <Form.List name="parameters">
                  {(fields, { add, remove }) => (
                    <>
                      {fields.map((field, index) => (
                        <Space key={field.key} style={{ display: 'flex', marginBottom: 8 }} align="baseline">
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
                      <Button type="dashed" onClick={() => add()} block icon={<PlusOutlined />}>
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
              shouldUpdate={(prevValues, currentValues) => prevValues.hasReturnValue !== currentValues.hasReturnValue}
            >
              {({ getFieldValue }) =>
                getFieldValue('hasReturnValue') && (
                  <Form.Item
                    name="returnType"
                    label="返回类型"
                    rules={[{ required: true, message: '请选择返回类型' }]}
                  >
                    <Select placeholder="请选择返回类型">
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
              rules={[{ required: true, message: '请选择数据类型' }]}
            >
              <Select placeholder="请选择数据类型">
                <Option value="int">整数 (int)</Option>
                <Option value="float">浮点数 (float)</Option>
                <Option value="string">字符串 (string)</Option>
                <Option value="bool">布尔值 (bool)</Option>
                <Option value="array">数组 (array)</Option>
                <Option value="object">对象 (object)</Option>
              </Select>
            </Form.Item>

            <Form.Item
              noStyle
              shouldUpdate={(prevValues, currentValues) => prevValues.dataType !== currentValues.dataType}
            >
              {({ getFieldValue }) => {
                const dataType = getFieldValue('dataType');
                return (
                  <Form.Item
                    name="defaultValue"
                    label="默认值"
                    rules={[{ required: true, message: '请输入默认值' }]}
                  >
                    {dataType === 'bool' ? (
                      <Select placeholder="请选择默认值">
                        <Option value={true}>true</Option>
                        <Option value={false}>false</Option>
                      </Select>
                    ) : dataType === 'int' ? (
                      <Input type="number" placeholder="请输入整数" />
                    ) : dataType === 'float' ? (
                      <Input type="number" step="0.01" placeholder="请输入浮点数" />
                    ) : (
                      <Input placeholder="请输入默认值" />
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
                    {fields.map((field, index) => (
                      <Space key={field.key} style={{ display: 'flex', marginBottom: 8 }} align="baseline">
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
                    <Button type="dashed" onClick={() => add()} block icon={<PlusOutlined />}>
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
                    {fields.map((field, index) => (
                      <Card
                        key={field.key}
                        size="small"
                        style={{ marginBottom: 16 }}
                        title={`方法 ${index + 1}`}
                        extra={
                          <Button type="link" danger onClick={() => remove(field.name)}>
                            删除
                          </Button>
                        }
                      >
                        <Form.Item
                          {...field}
                          name={[field.name, 'name']}
                          label="方法名"
                          rules={[{ required: true, message: '方法名不能为空' }]}
                        >
                          <Input placeholder="请输入方法名" />
                        </Form.Item>

                        <Form.Item label="参数列表">
                          <Form.List name={[field.name, 'parameters']}>
                            {(subFields, { add: addParam, remove: removeParam }) => (
                              <>
                                {subFields.map(subField => (
                                  <Space key={subField.key} style={{ display: 'flex', marginBottom: 8 }} align="baseline">
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
                                <Button type="dashed" onClick={() => addParam()} block icon={<PlusOutlined />}>
                                  添加参数
                                </Button>
                              </>
                            )}
                          </Form.List>
                        </Form.Item>

                        <Form.Item
                          {...field}
                          name={[field.name, 'returnType']}
                          label="返回类型"
                        >
                          <Input placeholder="请输入返回类型" />
                        </Form.Item>

                        <Form.Item
                          {...field}
                          name={[field.name, 'description']}
                          label="描述"
                        >
                          <Input.TextArea placeholder="请输入描述" rows={2} />
                        </Form.Item>
                      </Card>
                    ))}
                    <Button type="dashed" onClick={() => add()} block icon={<PlusOutlined />}>
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
            <Collapse defaultActiveKey={['parameters']}>
              <Panel header="参数列表" key="parameters">
                <Form.List name="parameters">
                  {(fields, { add, remove }) => (
                    <>
                      {fields.map((field, index) => (
                        <Space key={field.key} style={{ display: 'flex', marginBottom: 8 }} align="baseline">
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
                      <Button type="dashed" onClick={() => add()} block icon={<PlusOutlined />}>
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
              rules={[{ required: true, message: '请选择返回类型' }]}
            >
              <Select placeholder="请选择返回类型">
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
        title="显示面板"
        extra={
          <Space>
            <Input.Search
              placeholder="搜索符号名称"
              onSearch={setSearchText}
              style={{ width: 250 }}
            />
            <Tooltip title="导入">
              <Button icon={<ImportOutlined />} onClick={handleImport}>
                导入
              </Button>
            </Tooltip>
            <Tooltip title="导出">
              <Button icon={<ExportOutlined />} onClick={handleExport}>
                导出
              </Button>
            </Tooltip>
            <Button
              type="primary"
              icon={<PlusOutlined />}
              onClick={handleAdd}
            >
              新增符号
            </Button>
          </Space>
        }
      >
        <Table
          columns={columns}
          dataSource={symbols}
          rowKey="id"
          pagination={{
            pageSize: 10,
            showSizeChanger: true,
            showQuickJumper: true,
            showTotal: (total) => `共 ${total} 条记录`
          }}
          expandable={{
            expandedRowRender: (record: Symbol) => (
              <div style={{ margin: 0 }}>
                <p><strong>详细属性：</strong></p>
                <pre style={{
                  background: '#f5f5f5',
                  padding: 16,
                  borderRadius: 4,
                  maxHeight: 300,
                  overflow: 'auto'
                }}>
                  {JSON.stringify(record, null, 2)}
                </pre>
              </div>
            ),
          }}
        />
      </Card>

      <Modal
        title={editingSymbol ? '编辑符号' : '新增符号'}
        open={isModalVisible}
        onCancel={() => setIsModalVisible(false)}
        width={800}
        footer={null}
        destroyOnClose
      >
        <Form
          form={form}
          layout="vertical"
          onFinish={handleSubmit}
          initialValues={{
            type: 'macro-switch',
            references: '',
            hasReturnValue: false,
            parameters: [],
            members: [],
            methods: [],
          }}
        >
          <Form.Item
            name="name"
            label="名称"
            rules={[
              { required: true, message: '请输入符号名称' },
              { max: 50, message: '名称不能超过50个字符' }
            ]}
          >
            <Input placeholder="请输入符号名称" />
          </Form.Item>

          <Form.Item
            name="type"
            label="类型"
            rules={[{ required: true, message: '请选择类型' }]}
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
            <TextArea rows={2} placeholder="请输入描述" />
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
                {editingSymbol ? '更新' : '确定'}
              </Button>
            </Space>
          </Form.Item>
        </Form>
      </Modal>
    </div>
  );
};

export default DisplayPanel;
