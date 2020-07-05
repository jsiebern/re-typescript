let worker = WebWorkers.create_webworker("worker/worker.js")

type request_callback = (. Bridge_bs.worker_response_payload) => unit
let requests: Hashtbl.t<string, request_callback> = Hashtbl.create(0)

WebWorkers.onMessage(worker, (e: WebWorkers.MessageEvent.t) => {
  let (requestId, payload) = WebWorkers.MessageEvent.data(e)
  let callback = Hashtbl.find(requests, requestId)
  Hashtbl.remove(requests, requestId)
  callback(. payload)
})

let queryWorker = (payload: Bridge_bs.worker_request_payload, callback: request_callback) => {
  let requestId = Shortid.generate()
  Hashtbl.add(requests, requestId, callback)
  WebWorkers.postMessage(worker, (requestId, payload))
  requestId
}
