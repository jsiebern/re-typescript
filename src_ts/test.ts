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

// interface method_signature {
//     func1: (x: number) => number;
//     func2(x: number): number;
//     func3(x: string, y: boolean): void;
// }
// type extracted_method_signature = method_signature['func3' | 'func2'];


type obj = {
    field_1: number,
    field_2: {
        field_3: boolean
    }
};
type field_1 = obj['field_2']['field_3'];


`
);

file.forEachDescendant(node => {
    if (Node.isIndexedAccessTypeNode(node)) {

        // const index_node = node.getIndexTypeNode();
        // index_node.getChildren().forEach(child => {
        //     console.log('index_node > ', child.getKindName())
        // })
        // const obj_node = node.getObjectTypeNode();
        // obj_node.getChildren().forEach(child => {
        //     console.log('obj_node > ', child.getKindName())
        // })
        const parent = node.getParent();
        parent.getChildren().forEach(child => {
            console.log(child.getKindName())
        })
        // console.log('------')
    }
    if (node.getType().isBooleanLiteral()) {

    }
    // Node.isFunctionTypeNode
    // node.getSourceFile().getExportSymbols()
    // if (Node.isTypeReferenceNode(node)) {
    //     const checker = node.getProject().getTypeChecker();
    //     const xxx = checker.getSymbolAtLocation(node.getTypeName());
    //     console.log(xxx?.isAlias())
    //     console.log(xxx?.hasFlags(ts.SymbolFlags.TypeParameter))

    //     console.log(node.getTypeName().getText());
    // }

    // if (node.getKind() === ts.SyntaxKind.MappedType) {
    //     const tp = node.getLastChildByKindOrThrow(ts.SyntaxKind.TypeParameter);
    //     const c = tp.getConstraintOrThrow();
    //     const t = tp.getConstraintOrThrow().getType();
    //     if (Node.isEnumDeclaration((c))) {
    //         console.log(c.getMembers());
    //     }
    //     if (t && t.isEnum()) {
    //         console.log(t.getSymbol()?.getValueDeclarationOrThrow())
    //         console.log(t.getText())
    //     }
    // }
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
