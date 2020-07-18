import { Project, Node, TypeGuards } from 'ts-morph';
import * as c from '@ts-morph/common';

const project = new Project({
    useInMemoryFileSystem: true,
    compilerOptions: {
        noEmitOnError: false,
    },
});

const file = project.createSourceFile(
    'test.d.ts',
    `

interface KeyValueProcessor {
    (key: number, value: string): boolean;
}

`
);

file.forEachDescendant(node => {
    if (Node.isIntersectionTypeNode(node)) {

    }
    //     node.getExtends().forEach(ext => {
    //         ext.getTypeArguments
    //         const t = ext.getType();
    //         const s = t.getSymbol();
    //         if (s) {
    //             console.log('>> ', s.getDeclarations()[0].getText());
    //             const members = s.getMembers();
    //             members.forEach(memberSymbol => {
    //                 const decl = memberSymbol.getValueDeclaration();
    //                 if (decl) {
    //                     console.log('<><><> ', node.getProject().getTypeChecker().getTypeOfSymbolAtLocation(memberSymbol, decl).getText());

    //                     console.log('is ', decl.getType().isTypeParameter());
    //                     const p = decl.getParent();
    //                     if (p) {
    //                         console.log(p.getKindName())
    //                     }
    //                     console.log(decl.getKindName())
    //                 }
    //             })
    //         }
    //     })
    // }

});
