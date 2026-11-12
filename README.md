# URINOVA — Hackathon Prototype

English monitoring dashboard with fictional patients, a 20 cm experimental alert threshold, acknowledgement, resolution, and local alert history. Not for clinical use. There is no cloud API or Wi-Fi connection configured.

## النشر على GitHub Pages

1. أنشئي مستودعًا باسم `URINOVA` في GitHub. يمكن أن يكون عامًا لعرض بيانات المشروع الوهمية فقط.
2. ارفعي **محتويات هذا المجلد**، بحيث يكون `index.html` في جذر المستودع، وليس داخل مجلد إضافي. لا ترفعي ZIP نفسه.
3. افتحي Settings → Pages → Build and deployment.
4. اختاري Deploy from a branch، ثم main، ثم /(root)، واضغطي Save.
5. انتظري اكتمال النشر وافتحي الرابط الذي يعرضه GitHub. كل تحديث لاحق للملفات يحتاج Commit/Push حتى يظهر على الموقع.

رفع الكود هنا ليس نشرًا تم بالفعل. لا يوجد حساب أو مستودع GitHub مرتبط بهذه الحزمة.

## كود الحساس: الافتراض مهم

الكود المرفق مخصص مبدئيًا للوحة **ESP32 DevKit الكلاسيكية + HC-SR04** باستخدام Arduino IDE. ليس كودًا عامًا لكل حساس أو لكل لوحة. تحققي من الموديل قبل التوصيل؛ أرسلي اسمه إذا كان مختلفًا.

المسافة هي من واجهة الحساس إلى الأرض. ثبتي الحساس بمحاذاة أدنى نقطة في الكيس ومتجهًا للأرض، أو عايري SENSOR_OFFSET_CM. اختبري باستخدام مسطرة وأسـطح ثابتة. الفشل في استقبال صدى يعطي خطأ، وليس ارتفاعًا يساوي صفرًا.

### التوصيل لهذا الافتراض فقط

| HC-SR04 | ESP32 DevKit |
|---|---|
| VCC | مصدر 5V منظّم مناسب للوحة |
| GND | GND مشترك |
| TRIG | GPIO 5 |
| ECHO | عبر مقسّم جهد إلى GPIO 18 |

مقسّم الجهد: ECHO → مقاومة 10kΩ → نقطة GPIO18 → مقاومة 10kΩ → GND. يخفض 5V إلى نحو 2.5V. **لا توصلي ECHO ذي 5V مباشرة إلى ESP32.** أرقام الأرجل للوحة الكلاسيكية ويجب مطابقتها بلوحتك.

### رفع الكود وتجربة الموقع

1. ثبتي دعم لوحات ESP32 من Espressif في Boards Manager داخل Arduino IDE.
2. افتحي `firmware/URINOVA_HCSR04/URINOVA_HCSR04.ino`، واختاري اللوحة والمنفذ المناسبين ثم Upload.
3. للتحقق، افتحي Serial Monitor بسرعة 115200؛ يجب أن تظهر أسطر JSON بقراءات أو أخطاء.
4. **أغلقي Serial Monitor**، واتركي ESP32 موصولًا بكيبل USB بيانات.
5. افتحي رابط GitHub Pages في **Chrome على الكمبيوتر**، ثم Device Integration → Connect USB Sensor، واختاري منفذ اللوحة.
6. الحساس مربوط بالمريض الوهمي Sarah Ali، الغرفة 301 والسرير A. باقي المرضى محاكاة. لا تستخدمي بيانات مرضى حقيقية.
7. قرّبي سطحًا مسطحًا إلى 20 سم أو أقل لتنشيط التنبيه. استلمي النداء، ثم ارفعي المسافة فوق 20 سم قبل تأكيد المعالجة.

هذه الطريقة **USB وليست Wi-Fi**. لا تحتاج خادمًا إضافيًا. الموقع يستخدم Web Serial بإذن منك، ولا يرفع القياسات إلى GitHub. مصدر الأوقات ساعة الكمبيوتر ووقت استقبال الرسالة، وليس وقتًا موثقًا من الحساس. افصلي USB للعودة إلى المحاكاة. تنبيه أعلى اللوحة يعرض أول نداء مفتوح؛ بقية النداءات تظهر في Alert History.

## ما الذي يستمر في العمل؟

يبقى رابط GitHub Pages متاحًا طالما الاستضافة والمستودع مفعّلان. استقبال USB والتنبيهات يحتاجان الكمبيوتر والصفحة واللوحة في حالة تشغيل؛ إغلاق الصفحة يوقف المراقبة. سجل النداءات محلي لكل متصفح، وليس قاعدة بيانات مشتركة، وقد يُحذف بمسح بيانات المتصفح. لا توجد مراقبة في الخلفية أو إشعارات هاتف.

للربط عبر Wi-Fi لاحقًا: ESP32 → HTTPS API مستضاف ومصادق عليه → قاعدة بيانات → اللوحة عبر SSE/WebSocket أو polling. GitHub Pages يستضيف الواجهة فقط. لا تضعي كلمات مرور أو مفاتيح خاصة في هذا المستودع العام.

## حدود التحقق

تم التحقق من صياغة JavaScript ومنطق التنبيهات في النسخة الأصلية. لم يُجمّع firmware هنا لعدم توفر Arduino toolchain، ولم يُجرّب على حساس حقيقي أو منفذ USB حقيقي. يجب اختبار ومعايرة الجهاز قبل العرض. عتبة 20 سم تجريبية وليست توصية طبية.

## Sources

- https://docs.github.com/en/pages/getting-started-with-github-pages/configuring-a-publishing-source-for-your-github-pages-site
- https://developer.chrome.com/docs/capabilities/serial/
- https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
- https://learn.adafruit.com/ultrasonic-sonar-distance-sensors/pinouts
