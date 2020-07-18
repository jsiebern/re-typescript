import { Project, Node, TypeGuards } from 'ts-morph';

const project = new Project({
    useInMemoryFileSystem: true,
    compilerOptions: {
        noEmitOnError: false,
    },
});

const file = project.createSourceFile(
    'test.d.ts',
    `

interface I_a<A, B = boolean> {
    field_1: A;
    field_2: B;
    field_4: string;
}
interface I_b extends I_a<string> {
    field_3: number
}

`
);

file.forEachDescendant(node => {
    // if (Node.isTypeArgumentedNode(node)) {
    // }
    if (Node.isInterfaceDeclaration(node)) {
        node.getExtends().forEach(ext => {
            ext.getTypeArguments
            const t = ext.getType();
            const s = t.getSymbol();
            if (s) {
                console.log('>> ', s.getDeclarations()[0].getText());
                const members = s.getMembers();
                members.forEach(memberSymbol => {
                    const decl = memberSymbol.getValueDeclaration();
                    if (decl) {
                        console.log('<><><> ', node.getProject().getTypeChecker().getTypeOfSymbolAtLocation(memberSymbol, decl).getText());

                        console.log('is ', decl.getType().isTypeParameter());
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
