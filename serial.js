// Local USB input only; no measurements are uploaded to GitHub.
let serialPort=null,serialReader=null,serialActive=false,lastUsbReading=0;
const usbStatus=document.querySelector('#usb-status');
function showUsb(text){usbStatus.textContent=text;}
async function stopUsb(){serialActive=false;if(serialReader)await serialReader.cancel().catch(()=>{});}
document.querySelector('#usb-disconnect').onclick=stopUsb;
document.querySelector('#usb-connect').onclick=async()=>{
  if(serialActive)return;
  if(!('serial' in navigator)||!window.isSecureContext){showUsb('Use desktop Chrome on the HTTPS GitHub Pages link.');return}
  try{
    serialPort=await navigator.serial.requestPort();
    await serialPort.open({baudRate:115200});serialActive=true;lastUsbReading=0;
    if(timer){clearInterval(timer);timer=null;$('#auto').textContent='▷ Start auto simulation'}
    ['#apply','#scenario','#auto','#reset'].forEach(s=>$(s).disabled=true);
    showUsb('USB port open — waiting for valid readings. Other patients remain simulated.');
    serialReader=serialPort.readable.getReader();const decoder=new TextDecoder();let buffer='';
    while(serialActive){const {value,done}=await serialReader.read();if(done)break;buffer+=decoder.decode(value,{stream:true});if(buffer.length>8192)buffer='';let pos;
      while((pos=buffer.indexOf('\n'))>=0){const line=buffer.slice(0,pos).trim();buffer=buffer.slice(pos+1);let msg;try{msg=JSON.parse(line)}catch{continue}
        if(msg.deviceId!=='SIM-01')continue;
        if(msg.status!=='ok'){lastUsbReading=0;showUsb('USB sensor fault — last displayed height is not current.');continue}
        if(typeof msg.heightCm!=='number'||!Number.isFinite(msg.heightCm)||msg.heightCm<0||msg.heightCm>400)continue;
        lastUsbReading=Date.now();read('SIM-01',Math.round(msg.heightCm*10)/10);
        showUsb('LIVE USB · Sarah Ali · Room 301 / Bed A · '+msg.heightCm.toFixed(1)+' cm · '+time(Date.now())+' · Other patients simulated.');
        const card=$('#patients .patient');
        document.querySelectorAll('#patients .patient').forEach(c=>{if(c.textContent.includes('SIM-01')){const label=c.querySelector('.updated span');if(label)label.textContent='USB reading'}});
      }
    }
  }catch(error){showUsb(error.name==='NotFoundError'?'USB selection cancelled.':'USB connection failed. Close Arduino Serial Monitor, then reconnect.');}
  finally{serialActive=false;if(serialReader){serialReader.releaseLock();serialReader=null}if(serialPort){await serialPort.close().catch(()=>{});serialPort=null}['#apply','#scenario','#auto','#reset'].forEach(s=>$(s).disabled=false);showUsb('USB disconnected — displayed values may be old. Simulation available.');}
};
setInterval(()=>{if(serialActive&&lastUsbReading&&Date.now()-lastUsbReading>5000)showUsb('USB data stale — no valid reading for over 5 seconds. Last height is not current.');},1000);
