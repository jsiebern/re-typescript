import { Project, Node, TypeGuards, ts } from 'ts-morph';
import * as c from '@ts-morph/common';
import { isEnumDeclaration } from 'typescript';

const project = new Project({
    useInMemoryFileSystem: true,
    compilerOptions: {
        noEmitOnError: false,
    },
});

const file = project.createSourceFile(
    'test.d.ts',
    `

export enum Keys = {
    A;
    B;
    C;
};
type Flags = { [K in Keys]: { x: K } };

`
);

file.forEachDescendant(node => {
    if (node.getKind() === ts.SyntaxKind.MappedType) {
        const tp = node.getLastChildByKindOrThrow(ts.SyntaxKind.TypeParameter);
        const c = tp.getConstraintOrThrow();
        const t = tp.getConstraintOrThrow().getType();
        if (Node.isEnumDeclaration((c))) {
            console.log(c.getMembers());
        }
        if (t && t.isEnum()) {
            console.log(t.getSymbol()?.getValueDeclarationOrThrow())
            console.log(t.getText())
        }
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
