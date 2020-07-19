import { Project, Node, ts } from 'ts-morph';
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

    type PickX<T, K extends keyof T> = {
        [P in K]: T[P];
    };
    interface A {
      x: string;
      y: number;
      z: boolean;
    }

    type keys = 'x' | 'y';
    type stripped = PickX<A, keys>;

`
);

file.forEachDescendant(node => {
    if (Node.isTypeReferenceNode(node)) {
        const checker = node.getProject().getTypeChecker();
        const xxx = checker.getSymbolAtLocation(node.getTypeName());
        console.log(xxx?.isAlias())
        console.log(xxx?.hasFlags(ts.SymbolFlags.TypeParameter))

        console.log(node.getTypeName().getText());
    }

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
