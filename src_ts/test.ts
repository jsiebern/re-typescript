import { Project, Node } from 'ts-morph';

const project = new Project({
    useInMemoryFileSystem: true,
    compilerOptions: {
        noEmitOnError: false,
    },
});

const file = project.createSourceFile(
    'test.d.ts',
    `

interface I_b { field: string }
interface I_a extends I_b {}

`
);

file.forEachDescendant(node => {
    if (Node.isInterfaceDeclaration(node)) {
        node.getExtends().forEach(ext => {
            const t = ext.getType();
            const s = t.getSymbol();
            if (s) {
                console.log('>> ', s.getDeclarations()[0].getText());
                const members = s.getMembers();
                members.forEach(memberSymbol => {
                    const decl = memberSymbol.getValueDeclaration();
                    if (decl) {
                        const p = decl.getParent();
                        if (p) {
                            console.log(p.getKindName())
                        }
                        console.log(decl.getKindName())
                    }
                })
            }
        })
    }

});
