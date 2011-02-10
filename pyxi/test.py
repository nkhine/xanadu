from x88 import *

def verify(result, expected=1):
    if result != expected:
        raise ValueError, "result " + repr(result) + \
                          " did not match " + repr(expected)
    print "ok"

# tumbler arithmetic
a = Address([2,3,4])
b = Address(2,3,4)
verify(a, b)

c = Offset([0,0,1])
d = a + c
verify(a != d)
verify(d > a)
verify(d, Address(2,3,5))

# span comparison
s = Span(a, d)
t = Span(a, c)
verify(s, t)

d = Address(1,0,1,0,1,2)
v = VSpec(d, [s])
w = Span(Address(1,0,1,0,1,2,0,2,3,4), Address(1,0,1,0,1,2,0,2,3,5))
verify(v[0].globalize(), w)
verify(w.start.split(), (d, b))

class TestStream(XuStream):
    queries = [
        "\nP0~",
        "35~0.1.1.0.1.0.1~1~2~",
        "5~1~v~0.1.1.0.1.0.1~1~0.1.1~1.1500~",
        "28~1~v~0.1.1.0.1.0.1~1~0.1.1~0.1~",
        "1~0.1.1.0.1.0.1~",
        "30~1~v~0.1.1.0.1.0.1~1~0.1.513~1.1~0~0~0~",
        "18~3~0.1.1.0.1.0.2.0.2.22~",
        "18~2~0.1.1.0.1.0.2.0.2.22~",
        "35~0.1.1.0.1.0.2~1~2~",
        "5~1~v~0.1.1.0.1.0.2~1~0.1.1~1.1500~",
        "28~1~v~0.1.1.0.1.0.2~1~0.1.1~0.1~",
        "16~"]
    responses = [
        "\nP0~",
        "35~0.1.1.0.1.0.1~",
        "5~1~t10~abcdefghij",
        "28~1~v~0.1.1.0.1.0.1~1~0.1.504~1.49~" +
            "1~v~0.1.1.0.1.0.1~1~0.1.2~1.6~0~",
        "1~1~0.1.1~1.553~",
        "30~1~0.1.1.0.1.0.2.0.2.22~",
        "18~1~v~0.1.1.0.1.0.2~1~0.2.1~1.1~",
        "18~1~v~0.1.1.0.1.0.2~1~0.1.1~1.57~",
        "35~0.1.1.0.1.0.2~",
        "5~1~t10~klmnopqrst",
        "28~1~v~0.1.1.0.1.0.2~31~0.1.1~1.1~0.1.1~1.1~0.1.1~1.1~0.1.1~" +
            "1.1~0.1.1~1.1~0.1.75~1.9~0.1.102~1.35~0.1.143~1.11~0.1.232~1" +
            ".11~0.1.253~1.13~0.1.276~1.6~0.1.317~1.6~0.1.333~1.5~0.1.348" +
            "~1.5~0.1.363~1.5~0.1.447~1.4~0.1.492~1.6~0.1.542~1.9~0.1.569" +
            "~1.11~0.1.682~1.8~0.1.694~1.12~0.1.707~1.14~0.1.748~1.8~0.1." +
            "765~1.8~0.1.782~1.8~0.1.799~1.8~0.1.799~1.8~0.1.816~1.8~0.1." +
            "833~1.8~0.1.850~1.8~0.1.867~1.8~1~v~0.1.1.0.1.0.2~6~0.1.1~1." +
            "57~0.1.1~1.1~0.1.1~1.1~0.1.1~1.1~0.1.1~1.1~0.1.1~1.1~1~v~0.1" +
            ".1.0.1.0.2~5~0.1.1~1.1~0.1.1~1.1~0.1.1~1.1~0.1.1~1.1~0.1.1~1" +
            ".1~",
        "16~"]

    def __init__(self):
        self.index = 0
        self.inbuf = ""
        self.outbuf = ""

    def write(self, data):
        verify(self.index < len(self.queries))
        query = self.queries[self.index]
        length = len(query)
        self.inbuf = self.inbuf + data
        if len(self.inbuf) >= length:
            data, self.inbuf = self.inbuf[:length], self.inbuf[length:]
            verify(data, self.queries[self.index])
            self.outbuf = self.outbuf + self.responses[self.index]
            self.index = self.index + 1

    def read(self, length):
        verify(length <= len(self.outbuf))
        data, self.outbuf = self.outbuf[:length], self.outbuf[length:]
        return data

    def close(self):
        pass

x = XuSession(XuConn(TestStream()))
doca = Address(1,1,0,1,0,1)
mydoca = x.open_document(doca, READ_ONLY, CONFLICT_COPY)
vspan = Span(Address(1,1), Offset(0,1500))
specset = SpecSet(VSpec(mydoca, [vspan]))
adata = x.retrieve_contents(specset)

docspan = Span(Address(1,1), Offset(1))
specset = SpecSet(VSpec(mydoca, [docspan]))
asource, atarget, atype = x.retrieve_endsets(specset)

vspanset = x.retrieve_vspanset(mydoca)

charspan = Span(Address(1,513), Offset(0,1))
specfrom = SpecSet(VSpec(mydoca, [charspan]))
links = x.find_links(specfrom)

type = x.follow_link(links[0], LINK_TYPE)
target = x.follow_link(links[0], LINK_TARGET)

docb = target[0].docid
mydocb = x.open_document(docb, READ_ONLY, CONFLICT_COPY)
vspan = Span(Address(1,1), Offset(0,1500))
specset = SpecSet(VSpec(mydocb, [vspan]))
bdata = x.retrieve_contents(specset)

docspan = Span(Address(1,1), Offset(1))
specset = SpecSet(VSpec(mydocb, [docspan]))
bsource, btarget, btype = x.retrieve_endsets(specset)
x.quit()

verify(mydoca, Address(1, 1, 0, 1, 0, 1))
verify(adata, ['abcdefghij'])
verify(asource, SpecSet([VSpec(Address(1, 1, 0, 1, 0, 1),
                               [Span(Address(1, 504), Offset(0, 49))])]))
verify(atarget, SpecSet([VSpec(Address(1, 1, 0, 1, 0, 1),
                               [Span(Address(1, 2), Offset(0, 6))])]))
verify(atype, SpecSet([]))
verify(vspanset, VSpec(Address(1, 1, 0, 1, 0, 1),
                       [Span(Address(1, 1), Offset(0, 553))]))
verify(links, [Address(1, 1, 0, 1, 0, 2, 0, 2, 22)])
verify(type, SpecSet([VSpec(Address(1, 1, 0, 1, 0, 2),
                            [Span(Address(2, 1), Offset(0, 1))])]))
verify(target, SpecSet([VSpec(Address(1, 1, 0, 1, 0, 2),
                              [Span(Address(1, 1), Offset(0, 57))])]))

verify(mydocb, Address(1, 1, 0, 1, 0, 2))
verify(bdata, ['klmnopqrst'])
verify(bsource, SpecSet([VSpec(Address(1, 1, 0, 1, 0, 2),
                               [Span(Address(1, 1), Offset(0, 1)),
                                Span(Address(1, 1), Offset(0, 1)),
                                Span(Address(1, 1), Offset(0, 1)),
                                Span(Address(1, 1), Offset(0, 1)),
                                Span(Address(1, 1), Offset(0, 1)),
                                Span(Address(1, 75), Offset(0, 9)),
                                Span(Address(1, 102), Offset(0, 35)),
                                Span(Address(1, 143), Offset(0, 11)),
                                Span(Address(1, 232), Offset(0, 11)),
                                Span(Address(1, 253), Offset(0, 13)),
                                Span(Address(1, 276), Offset(0, 6)),
                                Span(Address(1, 317), Offset(0, 6)),
                                Span(Address(1, 333), Offset(0, 5)),
                                Span(Address(1, 348), Offset(0, 5)),
                                Span(Address(1, 363), Offset(0, 5)),
                                Span(Address(1, 447), Offset(0, 4)),
                                Span(Address(1, 492), Offset(0, 6)),
                                Span(Address(1, 542), Offset(0, 9)),
                                Span(Address(1, 569), Offset(0, 11)),
                                Span(Address(1, 682), Offset(0, 8)),
                                Span(Address(1, 694), Offset(0, 12)),
                                Span(Address(1, 707), Offset(0, 14)),
                                Span(Address(1, 748), Offset(0, 8)),
                                Span(Address(1, 765), Offset(0, 8)),
                                Span(Address(1, 782), Offset(0, 8)),
                                Span(Address(1, 799), Offset(0, 8)),
                                Span(Address(1, 799), Offset(0, 8)),
                                Span(Address(1, 816), Offset(0, 8)),
                                Span(Address(1, 833), Offset(0, 8)),
                                Span(Address(1, 850), Offset(0, 8)),
                                Span(Address(1, 867), Offset(0, 8))])]))
verify(btarget, SpecSet([VSpec(Address(1, 1, 0, 1, 0, 2),
                               [Span(Address(1, 1), Offset(0, 57)),
                                Span(Address(1, 1), Offset(0, 1)),
                                Span(Address(1, 1), Offset(0, 1)),
                                Span(Address(1, 1), Offset(0, 1)),
                                Span(Address(1, 1), Offset(0, 1)),
                                Span(Address(1, 1), Offset(0, 1))])]))
verify(btype, SpecSet([VSpec(Address(1, 1, 0, 1, 0, 2),
                             [Span(Address(1, 1), Offset(0, 1)),
                              Span(Address(1, 1), Offset(0, 1)),
                              Span(Address(1, 1), Offset(0, 1)),
                              Span(Address(1, 1), Offset(0, 1)),
                              Span(Address(1, 1), Offset(0, 1))])]))
