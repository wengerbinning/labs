


```js
const win = new BrowserWindow({
    width: 800,
    height: 600,
    frame: false,
    webPreferences: { preload: path.join(__dirname, 'preload.js') }
})
```




* BrowserWindow: <https://www.electronjs.org/docs/latest/api/browser-window>