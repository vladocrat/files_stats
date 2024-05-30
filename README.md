# files_stats

## Architecture

The architecture consists of:
- Frontend
   - QML components
   - QQuickPaintedItem
- Backend
   - Application controller
   - Thread worker
   - File parser
 
  ![arch drawio](https://github.com/vladocrat/files_stats/assets/47445848/7bd1095f-02dc-4575-a3a9-b44076722424)

  ## Use-case
  1. User selects a file.
  2. After pressing "start" button application controller starts worker that works in another thread.
  3. The worker notifies application controller about new chunks of data recieved from file parser. (1 chunk = 5 lines by default).
  4. Controller counts occurances of each word and notifies graph and progressbar qml components.
  5. User can pause file processing at any time, by pressing "pause" button, and later resume it by pressing "resume" button.
  6. User can stop file processing by pressing "stop" button.

![изображение](https://github.com/vladocrat/files_stats/assets/47445848/dd233d2a-34a3-4eb3-bbe5-6849089f33b9)
