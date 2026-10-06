// mobile_app_html.h — RDX Professional Colorimeter Mobile Web UI
// Generated from mobile_app.html, served from ESP32 WiFi AP at 192.168.4.1
// Fixes applied: &#916;E for delta-E symbol, rgbB id for RGB blue element,
//                PASS/FAIL text replacing corrupted symbols, /api/getstd endpoint

#ifndef MOBILE_APP_HTML_H
#define MOBILE_APP_HTML_H

#include <pgmspace.h>

const char MOBILE_APP[] PROGMEM = R"HTMLEOF(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>RDX Professional Colorimeter - Mobile Control</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 10px;
        }
        .app-container {
            max-width: 500px;
            margin: 0 auto;
            background: white;
            border-radius: 20px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            overflow: hidden;
            height: 100vh;
            display: flex;
            flex-direction: column;
        }
        .header {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 20px;
            text-align: center;
            box-shadow: 0 5px 15px rgba(0,0,0,0.2);
        }
        .header h1 { font-size: 28px; margin-bottom: 5px; font-weight: 700; }
        .header p  { font-size: 12px; opacity: 0.9; }
        .status-bar {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 12px 20px;
            background: #f5f7fa;
            border-bottom: 1px solid #e0e0e0;
            font-size: 12px;
        }
        .status-item { display: flex; align-items: center; gap: 8px; }
        .status-dot  { width: 8px; height: 8px; border-radius: 50%; background: #4caf50; }
        .status-dot.offline { background: #f44336; }
        .battery { display: flex; align-items: center; gap: 5px; }
        .battery-icon {
            width: 24px; height: 12px;
            border: 1px solid #666; border-radius: 2px; padding: 2px;
            position: relative;
        }
        .battery-fill { height: 100%; background: #4caf50; border-radius: 1px; transition: all 0.3s; }
        .battery-icon::after {
            content: ''; position: absolute;
            right: -4px; top: 4px; width: 2px; height: 4px;
            background: #666; border-radius: 1px;
        }
        .content { flex: 1; overflow-y: auto; padding: 20px; }
        .section  { margin-bottom: 25px; }
        .section-title {
            font-size: 14px; font-weight: 600; color: #333;
            margin-bottom: 12px; text-transform: uppercase; letter-spacing: 1px;
        }
        .color-display {
            width: 100%; height: 120px; border-radius: 10px;
            background: linear-gradient(45deg,#ddd 25%,transparent 25%,transparent 75%,#ddd 75%,#ddd);
            background-size: 20px 20px;
            background-position: 0 0, 10px 10px;
            background-color: #fafafa;
            position: relative; margin-bottom: 15px;
            box-shadow: inset 0 2px 8px rgba(0,0,0,0.1);
        }
        .color-sample {
            width: 100%; height: 100%; border-radius: 10px;
            box-shadow: 0 4px 12px rgba(0,0,0,0.15);
        }
        .lab-grid { display: grid; grid-template-columns: repeat(3,1fr); gap: 12px; margin-bottom: 15px; }
        .lab-value {
            background: #f5f7fa; padding: 15px; border-radius: 10px;
            text-align: center; border: 2px solid #e0e0e0; transition: all 0.3s;
        }
        .lab-value.highlight { border-color: #667eea; background: #f0f4ff; }
        .lab-label {
            font-size: 11px; color: #999; text-transform: uppercase;
            letter-spacing: 0.5px; margin-bottom: 8px; font-weight: 600;
        }
        .lab-number { font-size: 28px; font-weight: 700; color: #667eea; }
        .button-group { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; margin-bottom: 15px; }
        .button {
            padding: 15px; border: none; border-radius: 10px;
            font-size: 14px; font-weight: 600; cursor: pointer;
            transition: all 0.3s; text-transform: uppercase; letter-spacing: 0.5px;
            box-shadow: 0 4px 12px rgba(0,0,0,0.15);
        }
        .btn-primary {
            background: linear-gradient(135deg,#667eea 0%,#764ba2 100%);
            color: white; grid-column: 1 / -1;
        }
        .btn-primary:hover  { transform: translateY(-2px); box-shadow: 0 6px 20px rgba(102,126,234,0.4); }
        .btn-primary:active { transform: translateY(0); }
        .btn-secondary { background: #f5f7fa; color: #667eea; border: 2px solid #667eea; }
        .btn-secondary:hover { background: #667eea; color: white; }
        .btn-danger { background: #f44336; color: white; }
        .btn-danger:hover { background: #d32f2f; }
        .btn-success { background: #4caf50; color: white; }
        .btn-success:hover { background: #388e3c; }
        .loading {
            display: inline-block; width: 12px; height: 12px;
            border: 2px solid #f3f3f3; border-top: 2px solid #667eea;
            border-radius: 50%; animation: spin 1s linear infinite; margin-right: 5px;
        }
        @keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }
        .history-list { max-height: 200px; overflow-y: auto; }
        .history-item {
            padding: 12px; background: #f5f7fa; border-radius: 8px;
            margin-bottom: 10px; font-size: 12px;
            display: flex; justify-content: space-between; align-items: center;
            border-left: 4px solid #667eea;
        }
        .history-item-time   { color: #999; font-size: 11px; }
        .history-item-values { font-weight: 600; color: #333; margin-right: auto; margin-left: 10px; }
        .delta-e-result {
            background: #f5f7fa; padding: 15px; border-radius: 10px;
            text-align: center; margin-bottom: 15px; border: 2px solid #e0e0e0;
        }
        .delta-e-value  { font-size: 36px; font-weight: 700; color: #667eea; margin: 10px 0; }
        .delta-e-status { font-size: 12px; text-transform: uppercase; letter-spacing: 0.5px; font-weight: 600; }
        .pass { color: #4caf50; }
        .fail { color: #f44336; }
        .tabs {
            display: flex; background: #f5f7fa; border-bottom: 1px solid #e0e0e0;
            margin-bottom: 15px; border-radius: 10px; padding: 5px;
        }
        .tab {
            flex: 1; padding: 10px; text-align: center; cursor: pointer;
            font-size: 12px; font-weight: 600; border-radius: 8px;
            transition: all 0.3s; background: transparent; border: none; color: #999;
        }
        .tab.active { background: white; color: #667eea; box-shadow: 0 2px 8px rgba(0,0,0,0.1); }
        .footer {
            padding: 15px 20px; background: #f5f7fa; border-top: 1px solid #e0e0e0;
            text-align: center; font-size: 11px; color: #999;
        }
        @media (max-height: 700px) { .content { overflow-y: scroll; -webkit-overflow-scrolling: touch; } }
    </style>
</head>
<body>
<div class="app-container">
    <div class="header">
        <h1>RDX Colorimeter</h1>
        <p>Professional Color Measurement System</p>
    </div>

    <div class="status-bar">
        <div class="status-item">
            <div class="status-dot" id="connectionStatus"></div>
            <span id="connectionText">Connecting...</span>
        </div>
        <div class="battery">
            <span id="batteryPercent">--</span>%
            <div class="battery-icon">
                <div class="battery-fill" id="batteryFill" style="width:75%"></div>
            </div>
        </div>
    </div>

    <div class="content">
        <!-- Measurement -->
        <div class="section">
            <div class="section-title">Current Measurement</div>
            <div class="color-display">
                <div class="color-sample" id="colorSample" style="background:rgb(128,128,128)"></div>
            </div>
            <div class="lab-grid">
                <div class="lab-value">
                    <div class="lab-label">L*</div>
                    <div class="lab-number" id="labL">--</div>
                </div>
                <div class="lab-value">
                    <div class="lab-label">a*</div>
                    <div class="lab-number" id="labA">--</div>
                </div>
                <div class="lab-value">
                    <div class="lab-label">b*</div>
                    <div class="lab-number" id="labBval">--</div>
                </div>
            </div>
            <div class="lab-grid">
                <div class="lab-value" style="grid-column:1/-1">
                    <div class="lab-label">RGB Values</div>
                    <div style="display:flex;justify-content:space-around;margin-top:8px">
                        <div style="color:#d32f2f"><strong id="labR">--</strong></div>
                        <div style="color:#388e3c"><strong id="labG">--</strong></div>
                        <div style="color:#1976d2"><strong id="rgbB">--</strong></div>
                    </div>
                </div>
            </div>
            <div class="button-group">
                <button class="button btn-primary" onclick="scanColor()" id="scanBtn">
                    <span id="scanLoader"></span>Scan Color
                </button>
            </div>
        </div>

        <!-- Tabs -->
        <div class="tabs">
            <button class="tab active" onclick="switchTab('compare',this)">Compare</button>
            <button class="tab" onclick="switchTab('history',this)">History</button>
            <button class="tab" onclick="switchTab('settings',this)">Settings</button>
        </div>

        <!-- Compare -->
        <div id="compareTab" class="section">
            <div class="section-title">Color Comparison</div>
            <div class="button-group">
                <button class="button btn-secondary" onclick="loadStandard()">Load Standard</button>
                <button class="button btn-secondary" onclick="setAsStandard()">Set as Standard</button>
            </div>
            <div id="standardData" style="display:none;padding:12px;background:#f5f7fa;border-radius:8px;margin-bottom:15px">
                <div style="font-size:11px;color:#999;text-transform:uppercase;margin-bottom:8px">Standard Reference</div>
                <div style="font-size:14px;font-weight:600">
                    L: <span id="stdL">--</span> &nbsp; a: <span id="stdA">--</span> &nbsp; b: <span id="stdB">--</span>
                </div>
            </div>
            <button class="button btn-primary" onclick="compareWithStandard()" id="compareBtn">
                <span id="compareLoader"></span>Compare Now
            </button>
            <div id="deltaEResult" style="display:none">
                <div class="delta-e-result">
                    <div style="font-size:12px;color:#999;text-transform:uppercase">&#916;E (Color Difference)</div>
                    <div class="delta-e-value" id="deltaEValue">--</div>
                    <div class="delta-e-status" id="deltaEStatus">--</div>
                </div>
            </div>
        </div>

        <!-- History -->
        <div id="historyTab" class="section" style="display:none">
            <div class="section-title">Measurement History</div>
            <div id="historyList" class="history-list">
                <div style="text-align:center;color:#999;padding:20px;font-size:12px">No measurements yet</div>
            </div>
            <button class="button btn-danger" onclick="clearHistory()" style="margin-top:10px">Clear History</button>
        </div>

        <!-- Settings -->
        <div id="settingsTab" class="section" style="display:none">
            <div class="section-title">Device Settings</div>
            <div style="margin-bottom:15px">
                <label style="display:block;font-size:12px;color:#666;margin-bottom:8px">&#916;E Threshold</label>
                <input type="number" id="deltaEThreshold" min="0.1" max="50" step="0.1" value="2.0"
                       style="width:100%;padding:10px;border:1px solid #ddd;border-radius:8px;font-size:14px">
            </div>
            <div style="margin-bottom:15px">
                <label style="display:block;font-size:12px;color:#666;margin-bottom:8px">Color Space</label>
                <select id="colorSpace" style="width:100%;padding:10px;border:1px solid #ddd;border-radius:8px;font-size:14px">
                    <option value="LAB">CIE Lab</option>
                    <option value="LCH">CIE LCh</option>
                    <option value="RGB">RGB</option>
                    <option value="CMYK">CMYK</option>
                </select>
            </div>
            <div style="margin-bottom:15px">
                <label style="display:block;font-size:12px;color:#666;margin-bottom:8px">&#916;E Method</label>
                <select id="deltaEMethod" style="width:100%;padding:10px;border:1px solid #ddd;border-radius:8px;font-size:14px">
                    <option value="ab">&#916;E*ab</option>
                    <option value="94">&#916;E*94</option>
                    <option value="00">&#916;E*00</option>
                </select>
            </div>
            <button class="button btn-primary" onclick="saveSettings()">Save Settings</button>
            <button class="button btn-danger" onclick="resetDevice()" style="margin-top:8px;grid-column:1/-1;display:block;width:100%">Reset Device</button>
        </div>
    </div>

    <div class="footer">Version 1.0 | Firmware: 4.11 | SN: 173023025</div>
</div>

<script>
let currentData = {L:0,a:0,b:0,R:0,G:0,B:0};
let standardData = null;
let history = [];
let isConnected = false;

window.addEventListener('load', () => {
    checkConnection();
    setInterval(updateStatus, 3000);
});

function checkConnection() {
    fetch('/api/status')
        .then(r => r.json())
        .then(data => {
            isConnected = true;
            document.getElementById('connectionStatus').classList.remove('offline');
            document.getElementById('connectionText').textContent = 'Connected';
            document.getElementById('batteryPercent').textContent = data.battery || '--';
            document.getElementById('batteryFill').style.width = (data.battery || 75) + '%';
        })
        .catch(() => {
            isConnected = false;
            document.getElementById('connectionStatus').classList.add('offline');
            document.getElementById('connectionText').textContent = 'Offline - Join RDX_Colorimeter WiFi';
        });
}

function updateStatus() { checkConnection(); }

function scanColor() {
    if (!isConnected) { alert('Device not connected. Join RDX_Colorimeter WiFi.'); return; }
    document.getElementById('scanBtn').disabled = true;
    document.getElementById('scanLoader').innerHTML = '<span class="loading"></span>';
    fetch('/api/scan')
        .then(r => r.json())
        .then(data => {
            currentData = data;
            updateDisplay();
            addToHistory(data);
            document.getElementById('scanBtn').disabled = false;
            document.getElementById('scanLoader').innerHTML = '';
        })
        .catch(e => {
            alert('Scan failed: ' + e);
            document.getElementById('scanBtn').disabled = false;
            document.getElementById('scanLoader').innerHTML = '';
        });
}

function updateDisplay() {
    document.getElementById('labL').textContent    = (+currentData.L).toFixed(2);
    document.getElementById('labA').textContent    = (+currentData.a).toFixed(2);
    document.getElementById('labBval').textContent = (+currentData.b).toFixed(2);
    document.getElementById('labR').textContent    = Math.round(currentData.R);
    document.getElementById('labG').textContent    = Math.round(currentData.G);
    document.getElementById('rgbB').textContent    = Math.round(currentData.B);
    const rgb = 'rgb(' + Math.round(currentData.R) + ',' + Math.round(currentData.G) + ',' + Math.round(currentData.B) + ')';
    document.getElementById('colorSample').style.background = rgb;
}

function setAsStandard() {
    if (!currentData.L) { alert('Scan a color first'); return; }
    standardData = Object.assign({}, currentData);
    document.getElementById('standardData').style.display = 'block';
    document.getElementById('stdL').textContent = (+standardData.L).toFixed(2);
    document.getElementById('stdA').textContent = (+standardData.a).toFixed(2);
    document.getElementById('stdB').textContent = (+standardData.b).toFixed(2);
    fetch('/api/setstd?L=' + standardData.L + '&a=' + standardData.a + '&b=' + standardData.b);
    alert('Standard set successfully');
}

function compareWithStandard() {
    if (!standardData) { alert('Please set a standard first'); return; }
    document.getElementById('compareBtn').disabled = true;
    document.getElementById('compareLoader').innerHTML = '<span class="loading"></span>';
    fetch('/api/compare')
        .then(r => r.json())
        .then(data => {
            document.getElementById('deltaEResult').style.display = 'block';
            document.getElementById('deltaEValue').textContent = (+data.deltaE).toFixed(2);
            document.getElementById('deltaEStatus').className = 'delta-e-status ' + (data.pass ? 'pass' : 'fail');
            document.getElementById('deltaEStatus').textContent = data.pass ? 'PASS' : 'FAIL';
            document.getElementById('compareBtn').disabled = false;
            document.getElementById('compareLoader').innerHTML = '';
        })
        .catch(e => {
            alert('Comparison failed');
            document.getElementById('compareBtn').disabled = false;
            document.getElementById('compareLoader').innerHTML = '';
        });
}

function loadStandard() {
    fetch('/api/getstd')
        .then(r => r.json())
        .then(data => {
            if (data.L !== undefined) {
                standardData = data;
                document.getElementById('standardData').style.display = 'block';
                document.getElementById('stdL').textContent = (+data.L).toFixed(2);
                document.getElementById('stdA').textContent = (+data.a).toFixed(2);
                document.getElementById('stdB').textContent = (+data.b).toFixed(2);
            } else { alert('No standard saved on device'); }
        })
        .catch(() => alert('Load standard from device'));
}

function addToHistory(data) {
    const time = new Date().toLocaleTimeString();
    const item = { time: time, L: (+data.L).toFixed(1), a: (+data.a).toFixed(1), b: (+data.b).toFixed(1) };
    history.unshift(item);
    if (history.length > 50) history.pop();
    updateHistoryDisplay();
}

function updateHistoryDisplay() {
    const listEl = document.getElementById('historyList');
    if (history.length === 0) {
        listEl.innerHTML = '<div style="text-align:center;color:#999;padding:20px;font-size:12px">No measurements yet</div>';
        return;
    }
    listEl.innerHTML = history.map(item =>
        '<div class="history-item">' +
        '<div class="history-item-time">' + item.time + '</div>' +
        '<div class="history-item-values">L:' + item.L + ' a:' + item.a + ' b:' + item.b + '</div>' +
        '</div>'
    ).join('');
}

function clearHistory() {
    if (confirm('Clear all history?')) {
        history = [];
        updateHistoryDisplay();
        fetch('/api/reset');
    }
}

function switchTab(tabName, btn) {
    document.getElementById('compareTab').style.display  = 'none';
    document.getElementById('historyTab').style.display  = 'none';
    document.getElementById('settingsTab').style.display = 'none';
    document.getElementById(tabName + 'Tab').style.display = 'block';
    document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
    btn.classList.add('active');
}

function saveSettings() {
    const thr = document.getElementById('deltaEThreshold').value;
    fetch('/api/settings?threshold=' + thr).then(() => alert('Settings saved'));
}

function resetDevice() {
    if (confirm('Reset device to factory settings?')) {
        fetch('/api/reset').then(() => location.reload());
    }
}
</script>
</body>
</html>)HTMLEOF";

#endif // MOBILE_APP_HTML_H
