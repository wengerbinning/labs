import './content.css';

export default Content;


function Content() { return (
	<div className='content'>
		<label>iptables TRACE 指令生成器 </label>
		<form action="#"> <div>
			<div className='src-addr'>
				<input type='checkbox'></input>
				<label for="srcaddr">起始地址</label>
				<input id="srcaddr" type='text'
					pattern='^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$'
					title="Enter a valid IPv4 address (e.g., 192.168.1.1)"
				>
				</input>
			</div>
			<div className='dst-addr'>
				<input type='checkbox'></input>
				<label for="dstaddr">目的地址</label>
				<input id="dstaddr" type='text'>
				</input>
			</div>
			<div className='protocol'>
				<input type='checkbox'></input>
				<label for="protocol">数据协议</label>
				{/* <input id="protocol" type='text'></input> */}
				<input list="proto-list" />

				<datalist id="proto-list">
					<option value="ICMP" />
					<option value="TCP" />
					<option value="UDP" />
					<option value="IGMP" />
					<option value="PPP" />
				</datalist>
			</div>
			<div className='src-port'>
				<input type='checkbox'></input>
				<label for="srcport">起始端口</label>
				<input id="srcport" type='number'></input>
			</div>
			<div className='dst-port'>
				<input type='checkbox'></input>
				<label for="dstport">目的端口</label>
				<input id="dstport" type='number'></input>
			</div>
			<div className='action'> <fieldset > <div className='selector'>
				<legend>操作行为：</legend>
				<input type="radio" id="add" name="action" value="A" />
				<label for="add">添加</label>
				<input type="radio" id="del" name="action" value="D" />
				<label for="del">删除</label>
			</div> </fieldset> </div>
			<button type='submit'>生成指令</button>
		</div> </form>
		<div>iptables -t raw -A PREROUTING</div>
	</div>

)}